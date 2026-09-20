# Scotch Game Engine

A C++17 game engine and scene editor following [The Cherno's Hazel series](https://github.com/TheCherno/Hazel).

## Build and run

Install **Visual Studio 2026** with **Desktop development with C++**, **MSVC v145**, and a Windows SDK. From Developer PowerShell in the repository:

```powershell
git submodule update --init --recursive
.\Setup.bat
msbuild Scotch.slnx /m /t:Build /p:Configuration=Debug /p:Platform=x64
.\Run-Editor.bat
```

Alternatively, open **Scotch.slnx**, select **Debug / x64**, build, and press **F5**. `Scotch-Editor` is the startup project.

The launcher sets the working directory for `assets/`. It also accepts `Release` or `Dist` after building that configuration.

## Dependencies

Use the pinned submodules. **spdlog 1.14.1 / fmt 10.2.1** fixes compatibility with the newer MSVC standard library; the other vendor versions remain unchanged.

Bundled [Premake beta8](vendor/bin/premake/README.md) targets VS 2026. `Setup.bat vs2022` generates the v143 fallback; regenerate and rebuild when switching because both generators share project and output paths.

See the [visual stack audit](TECH_STACK_AUDIT.md) for decisions and verified results, and [smoke tests](tests/README.md) for regression checks.
