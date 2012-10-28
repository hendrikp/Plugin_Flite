/* Flite_Plugin - for licensing and copyright see license.txt */

#include <IPluginBase.h>

#pragma once

#define FLITE_VOICE_RMS "cmu_us_rms"
#define FLITE_VOICE_SLT "cmu_us_slt"

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
        virtual void AsyncSpeak( const char* sText, const char* sVoice = FLITE_VOICE_SLT ) = 0;
    };
};