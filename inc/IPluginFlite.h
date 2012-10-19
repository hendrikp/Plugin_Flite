/* Flite_Plugin - for licensing and copyright see license.txt */

#include <IPluginBase.h>

#pragma once

/**
* @brief Flite Plugin Namespace
*/
namespace FlitePlugin
{
    /**
    * @brief plugin Flite concrete interface
    */
    struct IPluginFlite
    {
        /**
        * @brief Get Plugin base interface
        */
        virtual PluginManager::IPluginBase* GetBase() = 0;

        /**
        * @brief Speak a text asynchronously
        * @param sText Text to Speak
        */
        virtual void AsyncSpeak( const char* sText ) = 0;
    };
};