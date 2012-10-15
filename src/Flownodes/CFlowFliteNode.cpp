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
                {0},
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
                    gPlugin->AsyncSpeak( GetPortString( pActInfo, EIP_TEXT ) );
                }

                break;
            }
        }
    };
}

REGISTER_FLOW_NODE_EX( "Flite_Plugin:TextToSpeech", FlitePlugin::CFlowFliteNode, CFlowFliteNode );
