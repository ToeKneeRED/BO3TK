@echo off

if exist "build" (
    echo [build.bat] Removing build folder...
    rmdir /s /q "build"
)

echo [build.bat] Recreating build folder...
mkdir "build"

if exist "vsxmake2022" (
    echo [build.bat] Removing vsxmake2022 folder...
    rmdir /s /q "vsxmake2022"
)

echo [build.bat] Generating project...
xmake project -k vsxmake

@REM echo [build.bat] Building...
@REM xmake -y

echo [build.bat] Setting up project dependencies...
xmake l project_deps.lua

echo Finished!

pause