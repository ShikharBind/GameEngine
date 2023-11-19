workspace "Scotch"
    architecture "x64"
    startproject "Scotch-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories related to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Scotch/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Scotch/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Scotch/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Scotch/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Scotch/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Scotch/vendor/entt/include"
IncludeDir["yaml"] = "%{wks.location}/Scotch/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Scotch/vendor/ImGuizmo"

group "Dependencies"
    include "Scotch/vendor/GLFW"
    include "Scotch/vendor/Glad"
    include "Scotch/vendor/imgui"
    include "Scotch/vendor/yaml-cpp"
group ""


group "Core"
    include "Scotch"
    include "Scotch-Editor"
group ""


group "Misc"
include "Sandbox"
group ""