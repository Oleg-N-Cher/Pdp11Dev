@ECHO OFF
SET PATH=..\..\Bin\gcc\bin
SET CC=..\..\Bin\gcc\libexec\gcc\pdp11-aout\10.0.1\cc1.exe
SET AS=pdp11-aout-as.exe
SET AR=pdp11-aout-ar.exe

SET Options=-Wno-write-strings -fverbose-asm -quiet -O2 -Os -fomit-frame-pointer
SET Include=-I ..\C -I ..

IF EXIST ..\C\%1.c GOTO Clib

:Olib
%CC% %Options% %Include% %1.c
IF errorlevel 1 PAUSE
GOTO Done

:Clib
%CC% %Options% %Include% ..\C\%1.c
IF errorlevel 1 PAUSE
MOVE /Y ..\C\%1.s %1.s >NUL

:Done
%AS% %1.s -o %1.o
IF errorlevel 1 PAUSE

%AR% -rc ..\CGLIB.lib %1.o
IF errorlevel 1 PAUSE
