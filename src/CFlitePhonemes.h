/* Flite_Plugin - for licensing and copyright see license.txt */

#pragma once

#include <CPluginFlite.h>

extern "C"
{
#include <flite.h>
};

namespace FlitePlugin
{


    string getPhonemes( const char* sText );
    float flite_text_to_speech_phenome( const char* text,
                                        cst_voice* voice,
                                        const char* outtype, void* pStream = NULL );
}