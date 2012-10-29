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
}