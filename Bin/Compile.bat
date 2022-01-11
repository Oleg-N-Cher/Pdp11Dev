@ECHO OFF
SET PdpDev=%XDev%\Pdp11Dev
IF "%XDev%"=="" SET PdpDev=..
SET PATH=%PdpDev%\Bin\gcc\bin
SET CC=%PdpDev%\Bin\gcc\libexec\gcc\pdp11-aout\11.2.1\cc1.exe
SET AS=pdp11-aout-as.exe

IF "%Clean%"=="" SET Clean=TRUE

SET Include=%Include% -I %PdpDev%\Lib\C -I %PdpDev%\Lib\Obj -I %PdpDev%\Lib
SET Options=-mbm1 -msoft-float -O2 -Os -quiet -fomit-frame-pointer -fverbose-asm -Wno-write-strings

%CC% -C %1.c %Options% %Include%
IF errorlevel 1 PAUSE
%AS% %1.s -o %1.o
IF errorlevel 1 PAUSE
IF "%Clean%"=="TRUE" DEL *.s
