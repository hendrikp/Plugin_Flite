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

        // TODO: Add your concrete interface declaration here
    };
};