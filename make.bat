@REM This is a "makefile." It compiles all of the .\luna\asm and .\luna\cpp
@REM files into a single executable. The project itself is explained in
@REM .\luna\readme\readme.

@REM -=Initialization=-

@ECHO off
echo Beginning MAKE for LunaOS.

set PD=C:\luna-os
set CS=C:\cpp
set CC=C:\cpp\tcc
set CL=C:\tasm\tlink

cd %CS%
@REM tcc -ml %PD%\luna.cpp
tcc -ml C:\luna-os\luna.cpp
del C:\luna-os\out\luna.exe
copy luna.exe C:\luna-os\out\luna.exe
del luna.*
echo .=-=-=-=-=-=-=-=-=-=-=-=.
:run
cd C:\luna-os
C:\luna-os\out\luna