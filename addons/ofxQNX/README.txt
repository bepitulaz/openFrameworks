PROJECT INFORMATION
===================
ofxQNX - BlackBerry PlayBook add-on for openFrameworks

Project page:
- http://www.multigesture.net/projects/ofxqnx/

Repository (developPlayBook branch): 
- https://github.com/falcon4ever/openFrameworks/tree/developPlayBook


Copyright (C) 2012 Laurence Muller
E-mail: laurence.muller@gmail.com
Website: http://www.multigesture.net


LICENSE
=======
New BSD License (3-clause license)
- see LICENSE.txt


INTRO
=====
This add-on attempts to bring the openFrameworks platform to the BlackBerry PlayBook. 

To compile the project you will need to install the BlackBerry Native SDK 2.0 (and Eclipse):
- https://bdsc.webapps.blackberry.com/native/


QUICK START
===========
Launch the IDE provided by the Native SDK and follow the instructions below:

First create a new workspace at:
- "openFrameworks\examples\qnx"

After the IDE is running, import the following projects: 
- "openFrameworks\libs"
- "openFrameworks\addons"
- "openFrameworks\addons\ofxQNX"
- The example projects under "openFrameworksPlayBook\examples\qnx"

Click on any of the example projects, build and run.


THIS FEELS LIKE ROCKETSCIENCE, HELP PLEASE!
===========================================
I have created a short screencast that shows you how to setup ofxQNX on Windows (other platforms should be similar). It covers everything you need to know to launch the examples.
- http://www.youtube.com/watch?v=CwQGHQN5fAA


BUILDING OPENFRAMEWORKS (from scratch)
======================================
Since it is a bit complicated to build the required dependencies, I have included binaries that are needed to build the openFrameworks "emptyExample" and placed them in "ofxQNX\libs" (For Device-Release, Device-Debug and Simulator-Debug).

If you still want to build these dependencies yourself (or use more recent versions of them), here are some hints:

* Linux/Windows/Mac?
While I am only testing this port on Windows, its easier to compile the dependencies for the QNX platform on a Linux machine. I recommend using a virtual machine (e.g. VirtualBox) with Ubuntu installed on it (and using the Linux version of the Native SDK).

* Setup the build environment
https://bdsc.webapps.blackberry.com/native/documentation/porting_overview_1970891_11.html

* Boost 
- http://www.boost.org/
- Included version is 1.49.0
- Required because "std::tr1::shared_ptr" and "std::tr1::weak_ptr" are not available from the Native SDK (#include <boost/tr1/memory.hpp>)
- The included version has been stripped down for use with openFrameworks.

* FreeImage 
- http://freeimage.sourceforge.net/
- Using version 3.14.1 and removed support for EXR to make it compile.

* GLU
- Since no GLU is included with the Native SDK I am using the one from:
- http://code.google.com/p/glues/
- Included version is 1.3
- This requires some patching (e.g. adding missing definitions)
- To compile: Import it to a "New BlackBerry OS C/C++ Project" and choose C, Managed Build, Static Lib (empty archive). After that just put in all the files from Sources and hit build. 

* Tess 2
- I am using the source files from "openFrameworks\libs\tess2\Sources"
- In tesselator.h you will need to add: "|| defined(__QNX__)" to make sure it will select: "typedef unsigned short TESSindex;"
- Compile the same way as GLU

* Poco
- http://pocoproject.org/
- Version 1.4.3
- This can only be compiled on a Linux machine. Included files are compiled on Ubuntu Linux 11.10
- To generate the makefiles: ./configure --config=QNX --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/home/falcon4ever/pocoplaybook
- You might need a bogus syslog.h (borrow from the Android NDK?)
- For the x86 build, add these extra flags: -m32 -march=i486
- Modify the QNX config as posted here: http://pocoproject.org/forum/viewtopic.php?f=12&t=5318
- Now you should be able to build it.
- Linker flags: -lPocoNet -lPocoUtil -lPocoXML -lPocoFoundation -lsocket

* SDL
- Add __PLAYBOOK__ to your project if you use Audio/SDL
- https://github.com/blackberry/SDL

* TouchControlOverlay
- https://github.com/blackberry/TouchControlOverlay

* Additional information can be found here:
http://forum.openframeworks.cc/index.php/topic,9189.msg42786.html#msg42786


NEWS
====
04/05/2012
- Updated project files
- Updated libraries for PlayBook and BB10 target
- Added instructions on how to build Poco and FreeImage for both platforms (including makefiles), check the Sources folder.

01/05/2012
- Updated code for screen dimensions
- Disabled keyboard gesture, you can still use the methods by calling the keyboard functions on-demand:
https://bdsc.webapps.blackberry.com/native/reference/com.qnx.doc.bps.lib_ref/com.qnx.doc.bps.lib_ref/topic/virtualkeyboard.h_functions_overview.html

26/04/2012
- Updated the instructions/readme and added a tutorial video.

26/04/2012
- Changed data dir.
- Private assets: ofToDataPath("", true) + "/app/native/assets/" (read-only)
- Private data: ofToDataPath("", true) + "/data/" (Contains the application's private data. The application has full access to read and write files in this folder.)
- Public data should be in: ofToDataPath("", true) + "/shared/XXX" (see link below for available subfolders)
-- More info: https://bdsc.webapps.blackberry.com/native/documentation/com.qnx.doc.ide.userguide/topic/files_base_.html

25/04/2012
- Implemented keyboard (use a downward swipe gesture from the top to open the on-screen keyboard)
- Updated qnxFontsExample example

10/04/2012
- ofxQNXSoundStream is now using SDL instead of QSA (Implemented by Seth). Seems to be providing low latency audio without distortions.

06/04/2012
- Added x86 binaries for use with the simulator
- Updated example project config files for the simulator (Set the project to Simulator-Debug)

04/04/2012
- Moved audio callback into its own thread (using ofThreads). Sound seems to be more stable, however its still noisy and seems to lag a bit.

02/04/2012
- Recompiled libtess2 with proper flags (this fixes the issues with qnxFontsExample and qnxPolygonExample)

01/04/2012
- Replaced freeimage with version 3.14.1 (This fixed the png issue with qnxImageLoaderExample)

31/03/2012
- Converted more examples to ofxQNX

30/03/2012 - beta
- Initial commit


FAQ
===
* Is ofxQNX stable enough for prime time?
- Yes, Definitely! It's currently used in NodeBeat (www.nodebeat.com) .

* Where do I store my files?
- Private assets: ofToDataPath("", true) + "/app/native/assets/" (read-only)
- Private data: ofToDataPath("", true) + "/data/" (Contains the application's private data. The application has full access to read and write files in this folder.)
- Public data should be in: ofToDataPath("", true) + "/shared/XXX" (see link below for available subfolders)
-- More info: https://bdsc.webapps.blackberry.com/native/documentation/com.qnx.doc.ide.userguide/topic/files_base_.html

* Camera access?
Currently the Native SDK doesnt provide access to the camera. More info about it here:
http://supportforums.blackberry.com/t5/Native-SDK-for-BlackBerry-Tablet/Camera-access/m-p/1398701
So unfortunately there wont be a qnxCameraExample or qnxOpenCVExample until then.


TODO
====
- Add patches required to compile the dependencies
- Add GPS support/example
- Implement ofxQNXSoundPlayer class (or try to use ofOpenALSoundPlayer)


BUGS?
=====
- Please report on github ;)


Examples
========
qnxAudioOutputExample 
- Runs ok
- Plays a generate waveform, touch makes modifies the waveform and position (Left/Right)

qnxEmptyExample
- Runs ok
- Use this project as a template 
- Open up .cproject, .project and bar-descriptor.xml and replace the text "qnxEmptyExample" with "yourprojectname"

qnxFontsExample
- Runs ok
- Example on how to use truetype fonts

qnxGraphicsExample 
- Runs ok 
- Basic graphics example

qnxImageLoaderExample 
- Runs ok
- Demonstrates how to use image resources in openFrameworks

qnxInputExample
- Runs ok 
- Demonstrates how to enable and use multitouch input in a project

qnxSoundPlayerExample
- No audio, ofxQNXSoundPlayer.h not implemented.
- Demonstrates how to play audio files

qnxPolygonExample 
- Runs ok
- Demonstrates how to draw (an animated) polygon

qnxTouchExample 
- Runs ok
- Demonstrates how to use the accelerometer (and touch input)

qnxVBOExample
- Runs ok
- Demonstrates how to use VBO in openFrameworks


PROJECT SETTINGS
================
These are already set in the example projects.

* Defines (-D)
- _FORTIFY_SOURCE=2
- USING_GL11
- TARGET_QNX
- TARGET_OPENGLES
- TARGET_LITTLE_ENDIAN
- __PLAYBOOK__

* Libraries (-l)
- bps
- screen
- EGL
- GLESv1_CM
- freetype
- png
- socket
- PocoNet
- PocoUtil
- PocoXML
- PocoFoundation
- freeimage
- tess2
- GLU
- asound