@echo off
setlocal
set "SCOTCH_CONFIGURATION=Debug"
if not "%~2"=="" goto usage
if "%~1"=="" goto run
if /I "%~1"=="Debug" goto run
if /I "%~1"=="Release" (
    set "SCOTCH_CONFIGURATION=Release"
    goto run
)
if /I "%~1"=="Dist" (
    set "SCOTCH_CONFIGURATION=Dist"
    goto run
)
:usage
echo Usage: %~nx0 [Debug^|Release^|Dist] 1>&2
exit /b 2

:run
set "SCOTCH_EDITOR=%~dp0bin\%SCOTCH_CONFIGURATION%-windows-x86_64\Scotch-Editor\Scotch-Editor.exe"
if not exist "%SCOTCH_EDITOR%" (
    echo ERROR: Build the %SCOTCH_CONFIGURATION% x64 configuration in Scotch.slnx first. 1>&2
    exit /b 1
)
pushd "%~dp0Scotch-Editor"
if errorlevel 1 exit /b 1
"%SCOTCH_EDITOR%"
set "SCOTCH_RUN_RESULT=%ERRORLEVEL%"
popd
exit /b %SCOTCH_RUN_RESULT%
