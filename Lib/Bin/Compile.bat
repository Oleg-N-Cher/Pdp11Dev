@ECHO OFF
SET PATH=..\..\Bin\gcc\bin
SET CC=..\..\Bin\gcc\libexec\gcc\pdp11-aout\11.2.1\cc1.exe
SET AS=pdp11-aout-as.exe
SET AR=pdp11-aout-ar.exe

SET Options=-mbm1 -O2 -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=-I ..\C -I ..

IF EXIST ..\C\%1.c GOTO Clib_C

:Olib_Obj

%CC% %Options% %Include% %1.c
IF errorlevel 1 PAUSE
GOTO Done

:Clib_C

IF EXIST ..\C\%1.oh DEL %1.oh
IF EXIST ..\C\%1.c DEL %1.c
%CC% %Options% %Include% ..\C\%1.c
IF errorlevel 1 PAUSE
MOVE /Y ..\C\%1.s %1.s >NUL

:Done
%AS% %1.s -o %1.o
IF errorlevel 1 PAUSE
::..\Bin\Clear.bat
