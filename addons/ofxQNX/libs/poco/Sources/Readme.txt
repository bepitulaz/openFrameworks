Source code:
https://sourceforge.net/projects/poco/files/sources/poco-1.4.3/poco-1.4.3p1.tar.gz/download

Instructions:
1. Unzip the package above
2. Copy all QNX-* files to [root]\build\config
3. Load the environment variables in the console (from the SDK folder):
   $ source bbndk-env.sh
4. Run one of the following commands (modify the prefix):

PlayBook target (ARMv7):
./configure --config=QNX-playbook-device --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/Users/lmuller/PocoPlayBook/ARMv7

PlayBook simulator (X86):
./configure --config=QNX-playbook-sim --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/Users/lmuller/PocoPlayBook/X86

BB10 target (ARMv7):
./configure --config=QNX-bb10-device --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/Users/lmuller/PocoBB10/ARMv7

BB10 simulator (X86):
./configure --config=QNX-bb10-sim --omit=NetSSL_OpenSSL,Crypto,Data/ODBC,Data/MySQL --no-tests --no-samples --static --prefix=/Users/lmuller/PocoBB10/X86

5. Type:
make -s

6. Install it:
make install
