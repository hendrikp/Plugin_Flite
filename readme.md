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
* ```fl_speak```
  Will speak the text supplied

Flownodes
=========
* ```Flite_Plugin:TextToSpeech```
  Will speak sText when Speak is triggered
