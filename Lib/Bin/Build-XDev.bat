::@ECHO OFF
SET PATH=..\..\Bin\gcc\bin
SET CC=..\..\Bin\gcc\libexec\gcc\pdp11-aout\11.2.1\cc1.exe
SET AS=pdp11-aout-as.exe
SET AR=pdp11-aout-ar.exe
SET SmartLib=..\..\..\Bin\smartlib.exe

SET Options=-mbm1 -msoft-float -O2 -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=-I ..\C -I ..

CD ..\Obj
IF EXIST ..\XDev.lib DEL ..\XDev.lib
IF EXIST ..\C\SYSTEM.c GOTO Clib_C

:Olib_Obj

%SmartLib% SYSTEM.c
GOTO Compile

:Clib_C

::IF EXIST ..\C\%1.oh DEL %1.oh
::IF EXIST ..\C\%1.c DEL %1.c
%SmartLib% ..\C\SYSTEM.c

:Compile

FOR %%i IN (SYSTEM_???.c) DO (
  %CC% %Options% %Include% %%i
  IF errorlevel 1 PAUSE
)

FOR %%i IN (SYSTEM_???.s) DO (
  %AS% %%i -o %%i.o
  IF errorlevel 1 PAUSE
)

FOR %%i IN (SYSTEM_???.s.o) DO %AR% -rc ..\XDev.lib %%i
..\Bin\Clear.bat
