/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginFlite.h>

#include <../flite/include/flite.h>

#include <PMUtils.hpp>

extern "C" {
    cst_voice* register_cmu_us_rms( const char* voxdir );
    void unregister_cmu_us_rms( cst_voice* v );
    cst_voice* register_cmu_us_slt( const char* voxdir );
    void unregister_cmu_us_slt( cst_voice* v );
}

#pragma comment(lib, "fliteDll.lib")
#pragma comment(lib, FLITE_VOICE_SLT ".lib")
#pragma comment(lib, FLITE_VOICE_RMS ".lib")

#define COMMAND_SPEAK "fl_speak"
#define COMMAND_SPEAKV "fl_speakv"

namespace FlitePlugin
{
    CPluginFlite* gPlugin = NULL;

    void Command_Speak( IConsoleCmdArgs* pArgs )
    {
        if ( !gPlugin )
        {
            return;
        }

        // Get Text to say
#undef GetCommandLine
        string sCommandLine( pArgs->GetCommandLine() );
        int nLen = sCommandLine.length();
        int nLenCmd = strlen( COMMAND_SPEAK " " );

        // Is text present?
        if ( nLen <= nLenCmd )
        {
            return;
        }

        // remove command
        string sText = sCommandLine.Mid( nLenCmd );

        // spawn thread to synthesize and speak
        gPlugin->AsyncSpeak( sText );
    };

    void Command_SpeakV( IConsoleCmdArgs* pArgs )
    {
        if ( !gPlugin )
        {
            return;
        }

        // Get Text to say
#undef GetCommandLine
        string sCommandLine( pArgs->GetCommandLine() );
        int nLen = sCommandLine.length();
        int nLenCmd = strlen( COMMAND_SPEAKV " " );

        // Is text present?
        if ( nLen <= nLenCmd || pArgs->GetArgCount() < 3 )
        {
            return;
        }

        // remove command
        string sText = sCommandLine.Mid( nLenCmd );

        // read over the voice
        size_t nOffset = sText.find_first_of( ' ' ) + 1;

        // remove paramters
        sText = sText.Mid( nOffset ).Trim();

        // spawn thread to synthesize and speak
        gPlugin->AsyncSpeak( sText, pArgs->GetArg( 1 ) );
    };

    CPluginFlite::CPluginFlite()
    {
        gPlugin = this;
    }

    CPluginFlite::~CPluginFlite()
    {
        Release( true );

        gPlugin = NULL;
    }

    bool CPluginFlite::Release( bool bForce )
    {
        bool bRet = true;

        if ( !m_bCanUnload )
        {
            // Should be called while Game is still active otherwise there might be leaks/problems
            bRet = CPluginBase::Release( bForce );

            if ( bRet )
            {
                // Unregister CVars
                if ( gEnv && gEnv->pConsole )
                {
                    gEnv->pConsole->RemoveCommand( COMMAND_SPEAK );
                    gEnv->pConsole->RemoveCommand( COMMAND_SPEAKV );
                }

                // Cleanup like this always (since the class is static its cleaned up when the dll is unloaded)
                gPluginManager->UnloadPlugin( GetName() );

                // Allow Plugin Manager garbage collector to unload this plugin
                AllowDllUnload();
            }
        }

        return bRet;
    };

    bool CPluginFlite::Init( SSystemGlobalEnvironment& env, SSystemInitParams& startupParams, IPluginBase* pPluginManager, const char* sPluginDirectory )
    {
        gPluginManager = ( PluginManager::IPluginManager* )pPluginManager->GetConcreteInterface( NULL );
        CPluginBase::Init( env, startupParams, pPluginManager, sPluginDirectory );

        // Register CVars/Commands
        if ( gEnv && gEnv->pConsole )
        {
            gEnv->pConsole->AddCommand( COMMAND_SPEAK, Command_Speak, VF_NULL, "Speak the text | "COMMAND_SPEAK" <Text...>" );
            gEnv->pConsole->AddCommand( COMMAND_SPEAKV, Command_SpeakV, VF_NULL, "Speak the text | "COMMAND_SPEAK" <Voice ("FLITE_VOICE_SLT" / "FLITE_VOICE_RMS")> <Text...>" );
        }

        return true;
    }

    const char* CPluginFlite::ListCVars() const
    {
        return COMMAND_SPEAK ",\n" COMMAND_SPEAKV ",\n";
    }

    const char* CPluginFlite::GetStatus() const
    {
        return "OK";
    }

    /**
    * @brief Speak text blocking
    * @param sText Text to speak
    * @param sVoice Voice to use
    * @return length
    */
    float BlockingSpeak( string sText, string sVoice = FLITE_VOICE_SLT )
    {
        if ( !sText )
        {
            return 0;
        }

        flite_init();
        cst_voice* v = NULL;

        if ( sVoice == FLITE_VOICE_RMS )
        {
            v = register_cmu_us_rms( NULL );
        }

        else //if(sVoice == FLITE_VOICE_SLT)
        {
            v = register_cmu_us_slt( NULL );
        }

        if ( v )
        {
            return flite_text_to_speech( sText, v, "play" );
        }

        return 0;
    }

    struct SAsyncSpeakData
    {
        string sText;
        string sVoice;
    };

    /**
    * @brief Internal Threadfunction for async speaking
    * @param sText Text to speak
    */
    unsigned int __stdcall _AsyncSpeak( void* pData )
    {
        SAsyncSpeakData* _pData = static_cast<SAsyncSpeakData*>( pData );
        BlockingSpeak( _pData->sText, _pData->sVoice );
        delete _pData;
        return 0;
    }

    // plugin concrete interface implementation
    void CPluginFlite::AsyncSpeak( const char* sText, const char* sVoice )
    {
        if ( !sText )
        {
            return;
        }

        // prepare for async thread
        SAsyncSpeakData* pData = new SAsyncSpeakData();
        pData->sText = SAFESTR( sText );
        pData->sVoice = SAFESTR( sVoice );

        // start thread
        unsigned int nThreadId = 0;
        void* pThread = ( void* )_beginthreadex( NULL, 0, _AsyncSpeak, static_cast<void*>( pData ), CREATE_SUSPENDED, &nThreadId );
        assert( pThread );
        ResumeThread( ( HANDLE )pThread );
    }
}