/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <Nodes/G2FlowBaseNode.h>

#include <CPluginFlite.h>

namespace FlitePlugin
{
    class CFlowFliteNode :
        public CFlowBaseNode<eNCT_Instanced>
    {
            enum EInputPorts
            {
                EIP_SPEAK = 0,
                EIP_TEXT,
                EIP_VOICE,
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

                config.pInputPorts = inputs;
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

                            gPlugin->AsyncSpeak( GetPortString( pActInfo, EIP_TEXT ), sVoice );
                        }

                        break;
                }
            }
    };
}

REGISTER_FLOW_NODE_EX( "Flite_Plugin:TextToSpeech", FlitePlugin::CFlowFliteNode, CFlowFliteNode );
