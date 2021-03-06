@ECHO OFF
SET PATH=..\..\Bin\gcc\bin
SET CC=..\..\Bin\gcc\libexec\gcc\pdp11-aout\11.2.1\cc1.exe
SET AS=pdp11-aout-as.exe
SET AR=pdp11-aout-ar.exe
SET SmartLib=..\..\..\Bin\smartlib.exe

SET Options=-mbm1 -msoft-float -O2 -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=-I ..\C -I ..

IF EXIST ..\%1.lib DEL ..\%1.lib
IF EXIST ..\C\%1.c GOTO Clib_C

:Olib_Obj

%SmartLib% %1.c
GOTO Compile

:Clib_C

IF EXIST ..\C\%1.oh DEL %1.oh
IF EXIST ..\C\%1.c DEL %1.c
%SmartLib% ..\C\%1.c

:Compile

FOR %%i IN (%1_???.c) DO (
  %CC% %Options% %Include% %%i
  IF errorlevel 1 PAUSE
)

FOR %%i IN (%1_???.s) DO (
  %AS% %%i -o %%i.o
  IF errorlevel 1 PAUSE
)

FOR %%i IN (%1_???.s.o) DO %AR% -rc ..\%1.lib %%i
..\Bin\Clear.bat
