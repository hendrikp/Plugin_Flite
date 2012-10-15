/* Flite_Plugin - for licensing and copyright see license.txt */

#pragma once

#include <Game.h>

#include <IPluginManager.h>
#include <IPluginBase.h>
#include <CPluginBase.hpp>

#include <IPluginFlite.h>

#define PLUGIN_NAME "Flite"
#define PLUGIN_CONSOLE_PREFIX "[" PLUGIN_NAME " " PLUGIN_TEXT "] " //!< Prefix for Logentries by this plugin

namespace FlitePlugin
{
    /**
    * @brief Provides information and manages the resources of this plugin.
    */
    class CPluginFlite :
        public PluginManager::CPluginBase,
        public IPluginFlite
    {
        public:
            CPluginFlite();
            ~CPluginFlite();

            // IPluginBase
            bool Release( bool bForce );

            int GetInitializationMode() const
            {
                return int( PluginManager::IM_Default );
            };

            bool Init( SSystemGlobalEnvironment& env, SSystemInitParams& startupParams, IPluginBase* pPluginManager, const char* sPluginDirectory );

            const char* GetVersion() const
            {
                return "1.0.0.0";
            };

            const char* GetName() const
            {
                return PLUGIN_NAME;
            };

            const char* GetCategory() const
            {
                return "Other";
            };

            const char* ListAuthors() const
            {
                return "Hendrik Polczynski <hendrikpolczyn at gmail dot com>";
            };

            const char* ListCVars() const;

            const char* GetStatus() const;

            const char* GetCurrentConcreteInterfaceVersion() const
            {
                return "1.0";
            };

            void* GetConcreteInterface( const char* sInterfaceVersion )
            {
                return static_cast < IPluginFlite* > ( this );
            };

            // IPluginFlite
            IPluginBase* GetBase()
            {
                return static_cast<IPluginBase*>( this );
            };

            void AsyncSpeak(const char* sText);
    };

    extern CPluginFlite* gPlugin;
}

/**
* @brief This function is required to use the Autoregister Flownode without modification.
*/
inline void GameWarning( const char* sFormat, ... ) PRINTF_PARAMS( 1, 2 );
inline void GameWarning(const char * sFormat, ...)
{
    va_list ArgList;
    va_start( ArgList, sFormat );
    FlitePlugin::gPlugin->LogV( ILog::eWarningAlways, sFormat, ArgList );
    va_end( ArgList );
};
