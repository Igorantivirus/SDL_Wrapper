@echo off
setlocal enableextensions enabledelayedexpansion

set "ROOT_DIR=%~dp0"
cd /d "%ROOT_DIR%"

if not defined INSTALL_BASE set "INSTALL_BASE=D:/dev/libraries/_install/SDLWrapper"

if defined CMAKE_ANDROID (
    set "CMAKE_ANDROID_EXE=%CMAKE_ANDROID%"
) else (
    set "CMAKE_ANDROID_EXE=cmake"
)

call :build_preset "android-arm64" "arm64-v8a" "%CMAKE_ANDROID_EXE%"
if errorlevel 1 exit /b %errorlevel%

call :build_preset "android-armeabi-v7a" "armeabi-v7a" "%CMAKE_ANDROID_EXE%"
if errorlevel 1 exit /b %errorlevel%

call :build_preset "android-x86" "x86" "%CMAKE_ANDROID_EXE%"
if errorlevel 1 exit /b %errorlevel%

call :build_preset "android-x86_64" "x86_64" "%CMAKE_ANDROID_EXE%"
if errorlevel 1 exit /b %errorlevel%

exit /b 0

:build_preset
set "PRESET=%~1"
set "INSTALL_SUFFIX=%~2"
set "CMAKE_EXE=%~3"

"%CMAKE_EXE%" --preset "%PRESET%"
if errorlevel 1 exit /b %errorlevel%

for %%C in (Debug Release) do (
    "%CMAKE_EXE%" --build "build/%PRESET%" --config %%C
    if errorlevel 1 exit /b %errorlevel%
    "%CMAKE_EXE%" --install "build/%PRESET%" --config %%C --prefix "%INSTALL_BASE%/%INSTALL_SUFFIX%/%%C"
    if errorlevel 1 exit /b %errorlevel%
)

exit /b 0

