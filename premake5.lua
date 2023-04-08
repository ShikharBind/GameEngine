workspace "Scotch"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories related to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Scotch/vendor/GLFW/include"

include "Scotch/vendor/GLFW"

project "Scotch"
    location "Scotch"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "shpch.h"
    pchsource "Scotch/src/shpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SH_PLATFORM_WINDOWS",
            "SH_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "SH_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SH_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "SH_DIST"
        optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Scotch/vendor/spdlog/include",
        "Scotch/src"
    }

    links 
    {
        "Scotch"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "SH_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "SH_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SH_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "SH_DIST"
        optimize "On"