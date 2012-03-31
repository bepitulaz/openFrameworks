PROJECT INFORMATION
===================
ofxQNX - BlackBerry PlayBook add-on for openFrameworks
Repository: https://github.com/falcon4ever/openFrameworks (developPlayBook branch)

Copyright (C) 2012 Laurence Muller
E-mail: laurence.muller@gmail.com
Website: http://www.multigesture.net


LICENSE
=======
New BSD License (3-clause license)
- LICENSE.txt


NOTES
=====
This is addon attempts to bring the openFrameworks platform to the BlackBerry PlayBook. 

To compile the project you will need to install the BlackBerry Native SDK 2.0 (and Eclipse):
- https://bdsc.webapps.blackberry.com/native/


[Building openFrameworks]
Since it is a bit complicated to build the required dependencies, I have included binaries that are needed to build the openFrameworks "emptyExample" under "ofxQNX\libs".

If you want to try it out for yourselfs, here are some pointers:

* Boost 
- http://www.boost.org/
- Required because "std::tr1::shared_ptr" and "std::tr1::weak_ptr" are not available from the Native SDK (#include <boost/tr1/memory.hpp>)

* FreeImage 
- http://freeimage.sourceforge.net/
- Using version 3.15.2 and removed support for TIFF and EXR to make it compile.

* GLU
- Since no GLU is included with the Native SDK I am using the one from:
- http://code.google.com/p/glues/
- This requires some patching (e.g. adding missing definitions)
- To compile: Import it to a "New BlackBerry OS C/C++ Project" and choose C, Managed Build, Static Lib (empty archive). After that just put in all the files from Sources and hit build. 

* Tess 2
- Grab the sources from "openFrameworks\libs\tess2\Sources"
- Compile the same way as GLU

* Poco
- http://pocoproject.org/
- This can only be compiled on a linux machine. Included files are compiled on Ubuntu Linux 11.10
- To generate the makefiles: ./configure --config=QNX --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/home/falcon4ever/pocoplaybook
- Modify the QNX config as posted here: http://pocoproject.org/forum/viewtopic.php?f=12&t=5318
- Now you should be able to build it.
- Linker flags: -lPocoNet -lPocoUtil -lPocoXML -lPocoFoundation -lsocket

Additional information can be found here:
http://forum.openframeworks.cc/index.php/topic,9189.msg42786.html#msg42786


[Examples]
Open up eclipse and create a new workspace at:
- "openFrameworks\examples\qnx"

Next import: 
- "openFrameworks\libs"
- "openFrameworks\addons"
- "openFrameworks\addons\ofxQNX"
- The example projects under "openFrameworksPlayBook\examples\qnx"

[Project settings]
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
30/03/2012 - beta
- Initial commit


TODO
====
- Add more examples and instructions on how to compile the dependencies.
- Recompile FreeImage
- Check GLUES

Examples
========
qnxInputExample - demonstrates how to use multitouch input
qnxGraphicsExample - basic graphics example
qnxAudioOutputExample - play generated wave (broken atm)
qnxPolygonExample - Shape A B C E G H and I are broken. Issue with openFrameworks or perhaps GLU?
qnxImageLoaderExample - jpg/gif works, need to fix png support (prob related to FreeImage?)
qnxTouchExample - demonstrates how to use the accelerometer (and touch input)
