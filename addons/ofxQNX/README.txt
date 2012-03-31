PROJECT INFORMATION
===================
ofxQNX - BlackBerry PlayBook add-on for openFrameworks
Repository (developPlayBook branch): 
- https://github.com/falcon4ever/openFrameworks/tree/developPlayBook

Copyright (C) 2012 Laurence Muller
E-mail: laurence.muller@gmail.com
Website: http://www.multigesture.net


LICENSE
=======
New BSD License (3-clause license)
- see LICENSE.txt


NOTES
=====
This add-on attempts to bring the openFrameworks platform to the BlackBerry PlayBook. 

To compile the project you will need to install the BlackBerry Native SDK 2.0 (and Eclipse):
- https://bdsc.webapps.blackberry.com/native/


[Building openFrameworks]
Since it is a bit complicated to build the required dependencies, I have included binaries that are needed to build the openFrameworks "emptyExample" and placed them in "ofxQNX\libs".

If you still want to build these dependencies yourself (or use more recent versions of them), here are some tips:

* Linux/Windows?
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
- Using version 3.15.2 and removed support for TIFF and EXR to make it compile.

* GLU
- Since no GLU is included with the Native SDK I am using the one from:
- http://code.google.com/p/glues/
- Included version is 1.3
- This requires some patching (e.g. adding missing definitions)
- To compile: Import it to a "New BlackBerry OS C/C++ Project" and choose C, Managed Build, Static Lib (empty archive). After that just put in all the files from Sources and hit build. 

* Tess 2
- I am using the source files from "openFrameworks\libs\tess2\Sources"
- Compile the same way as GLU

* Poco
- http://pocoproject.org/
- Version 1.4.3
- This can only be compiled on a linux machine. Included files are compiled on Ubuntu Linux 11.10
- To generate the makefiles: ./configure --config=QNX --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/home/falcon4ever/pocoplaybook
- Modify the QNX config as posted here: http://pocoproject.org/forum/viewtopic.php?f=12&t=5318
- Now you should be able to build it.
- Linker flags: -lPocoNet -lPocoUtil -lPocoXML -lPocoFoundation -lsocket

Additional information can be found here:
http://forum.openframeworks.cc/index.php/topic,9189.msg42786.html#msg42786


[Importing examples]
Open up Eclipse and create a new workspace at:
- "openFrameworks\examples\qnx"

Next import: 
- "openFrameworks\libs"
- "openFrameworks\addons"
- "openFrameworks\addons\ofxQNX"
- The example projects under "openFrameworksPlayBook\examples\qnx"

[Project settings]
(these are already set in the example projects)

* Defines (-D)
- _FORTIFY_SOURCE=2
- USING_GL11
- TARGET_QNX
- TARGET_OPENGLES
- TARGET_LITTLE_ENDIAN

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


NEWS
=======
31/03/2012
- Converted more examples to ofxQNX

30/03/2012 - beta
- Initial commit


TODO
====
- Add more examples
- Add instructions and patches required to compile the dependencies
- Add GPS example
- Include a more recent version of FreeImage (3.15.3?)
- Include a more recent version of GLUES (1.4?)
-- And maybe use GLUES's libtess instead of the one that is bundled with openFrameworks (tess2)
- Add keyboard support (useful for qnxFontsExample example) 
-- https://bdsc.webapps.blackberry.com/native/documentation/keyboard_using_1935244_11.html

[Bugs]
- Fix PNG loading issues (qnxImageLoaderExample)
-- Possibly related to FreeImage?
- Fix font issues (qnxFontsExample)
- Fix polygon winding issues (qnxPolygonExample)

[Camera access]
Currently the Native SDK doesnt provide access to the camera. More info about it here:
http://supportforums.blackberry.com/t5/Native-SDK-for-BlackBerry-Tablet/Camera-access/m-p/1398701
So unfortunately there wont be a qnxCameraExample or qnxOpenCVExample until then.

Examples
========
qnxAudioOutputExample 
- Runs, but audio is messed up
- Plays a generate waveform, touch makes modifies the waveform and position (Left/Right)

qnxEmptyExample
- Runs ok
- Use this project as a template 
- Open up .cproject, .project and bar-descriptor.xml and replace the text "qnxEmptyExample" with "yourprojectname"

qnxFontsExample
- Runs but text is messed up
- Example on how to use truetype fonts

qnxGraphicsExample 
- Runs ok 
- Basic graphics example

qnxImageLoaderExample 
- Runs fine for JPG and GIF but PNG file support seem to be broken. (FreeImage issue?)
- Demonstrates how to use image resources in openFrameworks

qnxInputExample
- Runs ok 
- Demonstrates how to enable and use multitouch input in a project

qnxPolygonExample 
- Runs but shapes A B C E G H and I are broken. Issue with openFrameworks or perhaps GLUES?
- Demonstrates how to draw (an animated) polygon

qnxTouchExample 
- Runs ok
- Demonstrates how to use the accelerometer (and touch input)

qnxVBOExample
- Runs ok
- Demonstrates how to use VBO in openFrameworks
