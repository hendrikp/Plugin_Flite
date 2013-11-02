/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>

#include <CPluginFlite.h>
#include <Nodes/G2FlowBaseNode.h>

#include <MultiThread_Containers.h>

namespace FlitePlugin
{
    class CFlowFliteNode :
        public CFlowBaseNode<eNCT_Instanced>
    {

            double m_fCurrentTime;
            SPhenomeTiming m_phoCurrentPhoneme;
            CryMT::queue<SPhenomeTiming> m_qPhonemeStream;

            enum EInputPorts
            {
                EIP_SPEAK = 0,
                EIP_TEXT,
                EIP_VOICE,
            };

            enum EOutputPorts
            {
                EOP_PHONEME = 0,
                EOP_WEIGHT,
                EOP_FADEDURATION,
                EOP_DURATION,
            };

        public:
            virtual void GetMemoryUsage( ICrySizer* s ) const
            {
                s->Add( *this );
            }

            virtual IFlowNodePtr Clone( SActivationInfo* pActInfo )
            {
                return new CFlowFliteNode( pActInfo );
            }

            CFlowFliteNode( SActivationInfo* pActInfo )
            {

            }

            virtual void GetConfiguration( SFlowNodeConfig& config )
            {
                static const SInputPortConfig inputs[] =
                {
                    InputPortConfig_Void( "Speak", _HELP( "Speak the text" ) ),
                    InputPortConfig<string>( "sText", "", _HELP( "Text to speak" ), "sText", _UICONFIG( "" ) ),
                    InputPortConfig<int>( "Voice", 1, _HELP( "Voice to use" ), "nVoice", _UICONFIG( "enum_int:"FLITE_VOICE_SLT"=1,"FLITE_VOICE_RMS"=2" ) ),
                    InputPortConfig_Null()
                };

                static const SOutputPortConfig outputs[] =
                {
                    OutputPortConfig<string>( "Phoneme", _HELP( "Morph Name" ) ),
                    OutputPortConfig<float>( "Weight", _HELP( "Morph Weight" ) ),
                    OutputPortConfig<float>( "FadeDuration", _HELP( "Fade in Seconds" ) ),
                    OutputPortConfig<float>( "Duration", _HELP( "Duration in Seconds" ) ),
                    OutputPortConfig_Null(),
                };

                config.pInputPorts = inputs;
                config.pOutputPorts = outputs;
                config.sDescription = _HELP( PLUGIN_CONSOLE_PREFIX " Text To Speech" );

                config.SetCategory( EFLN_APPROVED );
            }

            virtual void ProcessEvent( EFlowEvent evt, SActivationInfo* pActInfo )
            {
                switch ( evt )
                {
                    case eFE_Activate:
                        if ( gPlugin && IsPortActive( pActInfo, EIP_SPEAK ) )
                        {
                            string sVoice = FLITE_VOICE_SLT;

                            if ( GetPortInt( pActInfo, EIP_VOICE ) == 2 )
                            {
                                sVoice = FLITE_VOICE_RMS;
                            }

                            m_fCurrentTime = 0;
                            m_qPhonemeStream.clear();
                            gPlugin->AsyncSpeak( GetPortString( pActInfo, EIP_TEXT ), sVoice, ( void* )&m_qPhonemeStream );

                            pActInfo->pGraph->SetRegularlyUpdated( pActInfo->myID, true );
                        }

                        break;

                    case eFE_Update:
                        {
                            if ( !m_qPhonemeStream.empty() )
                            {
                                m_fCurrentTime += gEnv->pTimer->GetFrameTime();

                                m_phoCurrentPhoneme = m_qPhonemeStream.front();

                                if ( m_fCurrentTime >= m_phoCurrentPhoneme.fStart )
                                {
                                    if ( m_qPhonemeStream.try_pop( m_phoCurrentPhoneme ) )
                                    {
                                        ActivateOutput<float>( pActInfo, EOP_DURATION, m_phoCurrentPhoneme.fDuration );
                                        ActivateOutput<float>( pActInfo, EOP_FADEDURATION, 0.1 );
                                        ActivateOutput<float>( pActInfo, EOP_WEIGHT, m_phoCurrentPhoneme.fWeight );
                                        ActivateOutput<string>( pActInfo, EOP_PHONEME, m_phoCurrentPhoneme.sName );
                                    }
                                }
                            }
                        }
                        break;
                }
            }
    };
}

REGISTER_FLOW_NODE_EX( "Flite_Plugin:TextToSpeech", FlitePlugin::CFlowFliteNode, CFlowFliteNode );
