Flite Plugin for CryEngine
==========================
Use Text to Speech as placeholder for later recording for dialogs or keep it for computer voice or robots.

Relies on [Flite Text to Speech Engine](http://www.speech.cs.cmu.edu/flite) which is a faster and lightweight version of Festival.

Can be combined with the animation plugin to provide realtime lipsync for characters.

Installation / Integration
==========================
Use the Installer or extract the files to your CryEngine SDK Folder so that the Code and BinXX/Plugins directory match up.

This plugin requires the Plugin SDK to be installed.
The plugin manager will automatically load up the plugin when the game/editor is restarted or if you directly load it.

CVars / Commands
================
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
  * Out ```Phoneme```Morph Name
  * Out ```Weight``` Morph Weight
  * Out ```FadeDuration``` Fade in Seconds
  * Out ```Duration``` Duration in Seconds

Contributing
============
* See [Plugin SDK Wiki: Contribution Guideline](https://github.com/hendrikp/Plugin_SDK/wiki/Contribution-Guideline)
* [Wishes / Issues](https://github.com/hendrikp/Plugin_Flite/issues)