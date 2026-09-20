@echo off
setlocal

set "SCOTCH_GENERATOR=vs2026"
if not "%~2"=="" goto usage
if "%~1"=="" goto generate
if /I "%~1"=="vs2026" goto generate
if /I "%~1"=="vs2022" (
    set "SCOTCH_GENERATOR=vs2022"
    goto generate
)

:usage
echo Usage: %~nx0 [vs2026^|vs2022] 1>&2
exit /b 2

:generate
pushd "%~dp0"
if errorlevel 1 exit /b 1

if not exist "vendor\bin\premake\premake5.exe" (
    echo ERROR: The bundled Premake executable is missing. Restore vendor\bin\premake\premake5.exe. 1>&2
    popd
    exit /b 1
)

"vendor\bin\premake\premake5.exe" %SCOTCH_GENERATOR%
set "SCOTCH_SETUP_RESULT=%ERRORLEVEL%"
if not "%SCOTCH_SETUP_RESULT%"=="0" goto failed

if "%SCOTCH_GENERATOR%"=="vs2026" (
    echo Projects generated. Open Scotch.slnx in Visual Studio 2026.
) else (
    echo Projects generated. Open Scotch.sln in Visual Studio 2022.
)
popd
exit /b 0

:failed
echo ERROR: Premake project generation failed with exit code %SCOTCH_SETUP_RESULT%. 1>&2
popd
exit /b %SCOTCH_SETUP_RESULT%
