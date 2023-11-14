project "Scotch-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs 
    {
        "%{wks.location}/Scotch/vendor/spdlog/include",
        "%{wks.location}/Scotch/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
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