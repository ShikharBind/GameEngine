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
IncludeDir["Glad"] = "Scotch/vendor/Glad/include"
IncludeDir["ImGui"] = "Scotch/vendor/imgui"
IncludeDir["glm"] = "Scotch/vendor/glm"

include "Scotch/vendor/GLFW"
    filter "system:windows"
        staticruntime "On"

include "Scotch/vendor/Glad"
include "Scotch/vendor/imgui"


project "Scotch"
    location "Scotch"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "shpch.h"
    pchsource "Scotch/src/shpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs 
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SH_PLATFORM_WINDOWS",
            "SH_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "SH_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SH_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SH_DIST"
        runtime "Release"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "Scotch/src",
        "%{IncludeDir.glm}"
    }

    links 
    {
        "Scotch"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SH_PLATFORM_WINDOWS"
        }

        filter "configurations:Debug"
            defines "SH_DEBUG"
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            defines "SH_RELEASE"
            runtime "Release"
            optimize "on"
    
        filter "configurations:Dist"
            defines "SH_DIST"
            runtime "Release"
            optimize "on"