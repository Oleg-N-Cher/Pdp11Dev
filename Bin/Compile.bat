@ECHO OFF
SET PATH=..\Bin\gcc\bin
SET gcc=..\Bin\gcc\libexec\gcc\pdp11-aout\10.0.1\cc1.exe

SET Include=-I ..\Lib\C -I ..\Lib
SET Options=-mfloat32

%gcc% -C %1.c %Options% %Include%
PAUSE
