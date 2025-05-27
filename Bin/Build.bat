@ECHO OFF
SET PdpDev=%XDev%\Pdp11Dev
IF "%XDev%"=="" SET PdpDev=..
SET PATH=%PdpDev%\Bin\gcc\bin
SET CC=%PdpDev%\Bin\gcc\libexec\gcc\pdp11-aout\11.2.1\cc1.exe
SET AS=pdp11-aout-as.exe

IF "%MainMod%"=="" SET MainMod=%1
IF "%MainMod%"=="%1" GOTO Build

:Compile

SET SaveOptions=%Options%
SET SaveInclude=%Include%
CALL %PdpDev%\Bin\Compile.bat %1
SET Options=%SaveOptions%
SET Include=%SaveInclude%

:Build

IF "%Clean%"=="" SET Clean=TRUE
IF "%Start%"=="" SET Start=TRUE
IF "%Pause%"=="" SET Pause=FALSE
IF "%Target%"=="UKNC" GOTO UKNC
IF "%Target%"=="MK90" GOTO MK90

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::                                             BK                                             ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SET Emul=d:\WIN32APP\Emulz\GID-BK\BK.exe /B
SET Options=%Options% -mbm1 -msoft-float -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=%Include% -I %PdpDev%\Lib\C -I %PdpDev%\Lib\Obj -I %PdpDev%\Lib
SET Libraries=%Libraries% -L %PdpDev%\Lib %PdpDev%\Lib\XDev.lib %PdpDev%\Lib\Basic.lib %PdpDev%\Lib\Laser2.lib %PdpDev%\Lib\Control.lib

%CC% %Options% %Include% %MainMod%.c
IF errorlevel 1 PAUSE
%AS% %MainMod%.s -o %MainMod%.o
IF errorlevel 1 PAUSE
pdp11-aout-ld.exe -T %PdpDev%\Bin\pdp11-bin.ld %PdpDev%\Lib\crt0.o %MainMod%.o %Modules% %Libraries% -o %MainMod%.out
::pdp11-aout-ld.exe --entry 512 %PdpDev%\Lib\crt0.o %MainMod%.o %Modules% %Libraries% -o %MainMod%.bin
IF errorlevel 1 PAUSE
::pdp11-aout-objcopy.exe --only-section .text --output-target binary %MainMod%.out ..\%MainMod%.bin
pdp11-aout-objcopy.exe --output-target binary %MainMod%.out ..\%MainMod%.bin
IF errorlevel 1 PAUSE
%PdpDev%\Bin\make-bk.exe ..\%MainMod% 512
IF errorlevel 1 PAUSE
IF "%Clean%"=="TRUE" DEL *.out *.s
IF "%Start%"=="TRUE" START %Emul% ..\%MainMod%.bin
EXIT

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:                                            UKNC                                              :
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: http://ancientbits.blogspot.com/2012/07/programming-barebones-pdp11.html

SET Emul=d:\WIN32APP\Emulz\UKNCbtl\UKNCBTL.exe /autostart /boot /disk0:UKNC.dsk
SET CodeAdr=01000
SET Options=-mbm2 -msoft-float -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=-I %PdpDev%\Lib\C -I %PdpDev%\Lib\Obj -I %PdpDev%\Lib
SET Libraries=%Libraries% -L %PdpDev%\Lib %PdpDev%\Lib\CGLIB.lib %PdpDev%\Lib\Graph.lib

%CC% %Options% %Include% %MainMod%.c
IF errorlevel 1 PAUSE
%AS% %MainMod%.s -o %MainMod%.o
IF errorlevel 1 PAUSE
pdp11-aout-ld.exe %PdpDev%\Lib\crt0rt.o -T pdp11-aout.ld %MainMod%.o -o %MainMod%.out %Libraries%
IF errorlevel 1 PAUSE
bin2load.exe -a -f %MainMod%.out -o %MainMod%.lda -b %CodeAdr%
IF errorlevel 1 PAUSE
IF EXIST %MainMod%.sav DEL /F %MainMod%.sav
lda2sav.exe -o %MainMod%.sav %MainMod%.lda
IF errorlevel 1 PAUSE
IF NOT EXIST %MainMod%.sav EXIT
IF EXIST DEMUK.SAV DEL /F DEMUK.SAV
MOVE /Y %MainMod%.sav DEMUK.SAV >NUL
%PdpDev%\Bin\rt11dsk.exe d ..\UKNC.dsk DEMUK.SAV
IF errorlevel 1 PAUSE
%PdpDev%\Bin\rt11dsk.exe a ..\UKNC.dsk DEMUK.SAV
IF errorlevel 1 PAUSE
IF "%Start%"=="TRUE" START %Emul%
EXIT

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:                                             MK90                                            ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SET EmulPath=d:\WIN32APP\Emulz\MK90\
SET Options=-mbm2 -msoft-float -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings
SET Include=%Include% -I %PdpDev%\Lib\C -I %PdpDev%\Lib\Obj -I %PdpDev%\Lib
SET Libraries=%Libraries% -L %PdpDev%\Lib %PdpDev%\Lib\XDev.lib %PdpDev%\Lib\MK90.lib

%CC% %Options% %Include% %MainMod%.c
IF errorlevel 1 PAUSE
%AS% %MainMod%.s -o %MainMod%.o
IF errorlevel 1 PAUSE
pdp11-aout-ld.exe -T %PdpDev%\Bin\pdp11-bin0.ld --entry 0 %PdpDev%\Lib\crt0mk90.o %MainMod%.o %Modules% %Libraries% -o %MainMod%.out
IF errorlevel 1 PAUSE
pdp11-aout-objcopy.exe --output-target binary %MainMod%.out ..\%MainMod%.bin
IF errorlevel 1 PAUSE

IF "%Clean%"=="TRUE" DEL *.out *.s
IF "%Start%"\=="TRUE" EXIT
COPY /b ..\%MainMod%.bin %EmulPath%\smp0.bin
CD %EmulPath%
START mk90.exe
