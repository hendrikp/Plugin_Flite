Flite Plugin for CryEngine SDK
=====================================
Use Text to Speech as placeholder for later real sounds for dialogs or keep it for computer voice or robots.

For redistribution please see license.txt.

Relies on [Flite Text to Speech Engine](http://www.speech.cs.cmu.edu/flite) which is a faster and lightweight version of Festival.

Installation / Integration
==========================
Extract the files to your Cryengine SDK Folder so that the Code and BinXX/Plugins directory match up.

The plugin manager will automatically load up the plugin when the game/editor is restarted or if you directly load it.

CVars
=====
* ```fl_speak <Text..>```
  Will speak the text supplied

* ```fl_speakv <Voice> <Text..>```
  Will speak the text with the voice supplied
  * ```cmu_us_slt``` US female voice
  * ```cmu_us_rms``` US male voice

Flownodes
=========
* ```Flite_Plugin:TextToSpeech``` Will speak sText
  * In ```Speak``` Speak the text now
  * In ```sText``` Text to speak
  * In ```Voice``` Voice to use
