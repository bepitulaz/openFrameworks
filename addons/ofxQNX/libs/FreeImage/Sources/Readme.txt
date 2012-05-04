Source code:
http://sourceforge.net/projects/freeimage/files/Source%20Distribution/3.14.1/

Instructions:
1. Unzip the package above
2. Copy all Makefile.* files to the root
3. Copy Plugin.cpp to [root]\Source\FreeImage
4. Open up the target makefile and edit the DESTDIR variable
5. Load the environment variables in the console (from the SDK folder):
   $ source bbndk-env.sh

6. Then use one of these commands:

PlayBook target (ARMv7):
make -f Makefile.qnx-playbook-device

PlayBook simulator (X86):
make -f Makefile.qnx-playbook-sim

BB10 target (ARMv7):
make -f Makefile.qnx-bb10-device

BB10 simulator (X86):
make -f Makefile.qnx-bb10-sim

7. Install it:
make -f Makefile.[target] install


