/* Flite_Plugin - for licensing and copyright see license.txt */

#include <StdAfx.h>
#include <CPluginFlite.h>

#include <../flite/include/flite.h>

extern "C" {
    cst_voice* register_cmu_us_rms( const char* voxdir );
    void unregister_cmu_us_rms( cst_voice* v );
}

#pragma comment(lib, "../flite/lib/fliteDll.lib")
#pragma comment(lib, "../flite/lib/cmu_us_rms.lib")

#define COMMAND_SPEAK "fl_speak"

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

    CPluginFlite::CPluginFlite()
    {
        gPlugin = this;
    }

    CPluginFlite::~CPluginFlite()
    {
        gPlugin = NULL;
    }

    bool CPluginFlite::Release( bool bForce )
    {
        // Should be called while Game is still active otherwise there might be leaks/problems
        bool bRet = CPluginBase::Release( bForce );

        if ( bRet )
        {
            // Unregister CVars
            if ( gEnv && gEnv->pConsole )
            {
                gEnv->pConsole->RemoveCommand( COMMAND_SPEAK );
            }

            // Cleanup like this always (since the class is static its cleaned up when the dll is unloaded)
            gPluginManager->UnloadPlugin( GetName() );

            // Allow Plugin Manager garbage collector to unload this plugin
            AllowDllUnload();
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
            gEnv->pConsole->AddCommand( COMMAND_SPEAK, Command_Speak, VF_NULL, "Speak the text" );
        }

        return true;
    }

    const char* CPluginFlite::ListCVars() const
    {
        return COMMAND_SPEAK "\n";
    }

    const char* CPluginFlite::GetStatus() const
    {
        return "OK";
    }

    /**
    * @brief Speak text blocking
    * @param sText Text to speak
    * @return length
    */
    float BlockingSpeak( const char* sText )
    {
        if ( !sText )
        {
            return 0;
        }

        flite_init();
        cst_voice* v = register_cmu_us_rms( NULL );
        return flite_text_to_speech( sText, v, "play" );
    }

    /**
    * @brief Internal Threadfunction for async speaking
    * @param sText Text to speak
    */
    unsigned int __stdcall _AsyncSpeak( void* sText )
    {
        BlockingSpeak( static_cast<const char*>( sText ) );
        delete [] sText;
        return 0;
    }

    // plugin concrete interface implementation
    void CPluginFlite::AsyncSpeak( const char* sText )
    {
        if ( !sText )
        {
            return;
        }

        // prepare for async thread
        char* pText = new char[ strlen( sText ) + 1];
        strcpy( pText, sText );

        // start thread
        unsigned int nThreadId = 0;
        void* pThread = ( void* )_beginthreadex( NULL, 0, _AsyncSpeak, static_cast<void*>( pText ), CREATE_SUSPENDED, &nThreadId );
        assert( pThread );
        ResumeThread( ( HANDLE )pThread );
    }
}