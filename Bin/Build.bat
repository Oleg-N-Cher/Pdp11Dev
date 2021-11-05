@ECHO OFF
:: http://ancientbits.blogspot.com/2012/07/programming-barebones-pdp11.html

SET CodeAdr=01000
SET Options=-Wno-write-strings -fverbose-asm -quiet -O2 -Os -fomit-frame-pointer
SET Include=-I ..\Lib\C -I ..\Lib\Obj -I ..\Lib
SET Libraries=%Libraries% -L ..\Lib ..\Lib\CGLIB.lib

SET PATH=..\Bin\gcc\bin
SET CC=..\Bin\gcc\libexec\gcc\pdp11-aout\10.0.1\cc1.exe
SET AS=pdp11-aout-as.exe

%CC% %Options% %Include% %1.c
IF errorlevel 1 PAUSE
%AS% %1.s -o %1.o
IF errorlevel 1 PAUSE
::pdp11-aout-ld --entry %CodeAdr% -T ldaout.cmd %1.o -o %1.out
pdp11-aout-ld --entry %CodeAdr% -T ldaout.cmd %1.o -o %1.out %Libraries%
IF errorlevel 1 PAUSE
bin2load -a -f %1.out -o %1.lda -b %CodeAdr%
IF errorlevel 1 PAUSE
IF EXIST %1.sav DEL /F %1.sav
lda2sav.exe -o %1.sav %1.lda
IF errorlevel 1 PAUSE

IF NOT EXIST %1.sav EXIT
IF EXIST DEMUK.SAV DEL /F DEMUK.SAV
MOVE /Y %1.sav DEMUK.SAV >NUL
..\Bin\rt11dsk.exe d ..\UKNC.dsk DEMUK.SAV
IF errorlevel 1 PAUSE
..\Bin\rt11dsk.exe a ..\UKNC.dsk DEMUK.SAV
IF errorlevel 1 PAUSE

::SET CURPATH=%CD%
::CD d:\WIN32APP\Emulz\bkbtl-335
::BKBTL.exe %CURPATH%\%1.bin
IF errorlevel 1 PAUSE

START d:\WIN32APP\Emulz\UKNCbtl\UKNCBTL.exe /autostart /boot
