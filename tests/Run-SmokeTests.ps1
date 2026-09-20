param(
    [string]$VisualStudioPath
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$outputDirectory = Join-Path $projectRoot 'bin/SmokeTests'
$debugDirectory = 'bin/Debug-windows-x86_64'

if (-not $VisualStudioPath) {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere)) {
        throw 'vswhere was not found. Pass -VisualStudioPath with your VS 2026 installation directory.'
    }
    $VisualStudioPath = & $vswhere -latest -prerelease -products '*' -version '[18.0,19.0)' `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $VisualStudioPath) {
        throw 'Visual Studio 2026 with C++ tools was not found.'
    }
}
$developerCommand = Join-Path $VisualStudioPath 'Common7/Tools/VsDevCmd.bat'
if (-not (Test-Path -LiteralPath $developerCommand)) {
    throw "VsDevCmd.bat was not found: $developerCommand"
}

$libraries = @(
    "$debugDirectory/Scotch/Scotch.lib",
    "Scotch/vendor/GLFW/$debugDirectory/GLFW/GLFW.lib",
    "Scotch/vendor/Glad/$debugDirectory/Glad/Glad.lib",
    "Scotch/vendor/imgui/$debugDirectory/ImGui/ImGui.lib",
    "Scotch/vendor/yaml-cpp/$debugDirectory/yaml-cpp/yaml-cpp.lib"
) | ForEach-Object { Join-Path $projectRoot $_ }
foreach ($library in $libraries) {
    if (-not (Test-Path -LiteralPath $library)) {
        throw "Missing $library. Run Setup.bat and build Debug x64 before running these tests."
    }
}

New-Item -ItemType Directory -Path $outputDirectory -Force | Out-Null
$executable = Join-Path $outputDirectory 'SmokeTests.exe'
$responseFile = Join-Path $outputDirectory 'compile.rsp'
$buildScript = Join-Path $outputDirectory 'compile.cmd'
$arguments = @(
    '/nologo', '/std:c++17', '/EHsc', '/MDd', '/Zi', '/Od', '/W3',
    '/D_DEBUG', '/DSH_DEBUG', '/DSH_PLATFORM_WINDOWS', '/DGLFW_INCLUDE_NONE', '/DYAML_CPP_STATIC_DEFINE',
    ('/Fo"{0}"' -f (Join-Path $outputDirectory 'SmokeTests.obj')),
    ('/Fd"{0}"' -f (Join-Path $outputDirectory 'SmokeTests-compile.pdb')),
    ('/Fe"{0}"' -f $executable)
)
foreach ($include in @('Scotch/src', 'Scotch/vendor/spdlog/include', 'Scotch/vendor/glm',
    'Scotch/vendor/entt/include', 'Scotch/vendor/GLFW/include', 'Scotch/vendor/Glad/include',
    'Scotch/vendor/yaml-cpp/include')) {
    $arguments += '/I"{0}"' -f (Join-Path $projectRoot $include)
}
$arguments += '"{0}"' -f (Join-Path $PSScriptRoot 'SmokeTests.cpp')
$arguments += '/link', '/DEBUG', '/INCREMENTAL:NO'
$arguments += $libraries | ForEach-Object { '"{0}"' -f $_ }
$arguments += 'opengl32.lib', 'user32.lib', 'gdi32.lib', 'shell32.lib', 'ole32.lib', 'comdlg32.lib',
    'advapi32.lib', 'imm32.lib', 'dwmapi.lib'
Set-Content -LiteralPath $responseFile -Value $arguments -Encoding Unicode
Set-Content -LiteralPath $buildScript -Encoding ASCII -Value @'
@echo off
setlocal
set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
call "%SCOTCH_SMOKE_VSDEVCMD%" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 1
cl @"%SCOTCH_SMOKE_RESPONSE%"
exit /b %errorlevel%
'@

$previousDeveloperCommand = $env:SCOTCH_SMOKE_VSDEVCMD
$previousResponseFile = $env:SCOTCH_SMOKE_RESPONSE
try {
    $env:SCOTCH_SMOKE_VSDEVCMD = $developerCommand
    $env:SCOTCH_SMOKE_RESPONSE = $responseFile
    & $buildScript
    if ($LASTEXITCODE -ne 0) { throw "Smoke test compilation failed with exit code $LASTEXITCODE." }
} finally {
    $env:SCOTCH_SMOKE_VSDEVCMD = $previousDeveloperCommand
    $env:SCOTCH_SMOKE_RESPONSE = $previousResponseFile
}

Push-Location -LiteralPath (Join-Path $projectRoot 'Scotch-Editor')
try {
    & $executable (Join-Path $outputDirectory 'artifacts') (Join-Path $projectRoot 'Scotch-Editor/assets/scenes')
    if ($LASTEXITCODE -ne 0) { throw "Smoke tests failed with exit code $LASTEXITCODE." }
} finally {
    Pop-Location
}
