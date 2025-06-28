include_dir = {}
include_dir["KyrnnessCore"]        = "%{wks.location}/Kyrnness/KyrnnessCore/Sources"
include_dir["KyrnnessSteamModule"] = "%{wks.location}/Kyrnness/KyrnnessSteamModule/include"
include_dir["KyrnnessPacker"]       = "%{wks.location}/Kyrnness/KyrnnessPacker/include"

include_dir["SDL"]             = "%{wks.location}/ThirdParty/SDL/include"
include_dir["GLM"]              = "%{wks.location}/ThirdParty/GLM/glm"
include_dir["PHYSX"]            = "F:/PhysX/physx/include"
include_dir["VULKAN"]           = "C:/VulkanSDK/1.4.309.0/Include"
include_dir["GLAD"]             = "%{wks.location}/ThirdParty/GLAD/include"
include_dir["IMGUI"]            = "%{wks.location}/ThirdParty/IMGUI"
include_dir["ASSIMP"]           = "%{wks.location}/ThirdParty/ASSIMP/include"
include_dir["ASSIMP_BUILD"]     = "%{wks.location}/ThirdParty/ASSIMP/build/include"
include_dir["STB"]              = "%{wks.location}/ThirdParty/STB"
include_dir["ENTT"]             = "%{wks.location}/ThirdParty/ENTT/src"
include_dir["TINY"]             = "%{wks.location}/ThirdParty/TINY"
include_dir["TINYOBJ"]          = "%{wks.location}/ThirdParty/TINYOBJ"
include_dir["GLFW"]             = "%{wks.location}/ThirdParty/GLFW/include"
include_dir["JSON"]             = "%{wks.location}/ThirdParty/JSON/include"
include_dir["SteamSDK"]         = "%{wks.location}/ThirdParty/SteamSDK/public"
include_dir["FMOD"]             = "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc"
include_dir["ZLIB"]             = "%{wks.location}/ThirdParty/ZLIB"
include_dir["FreeType"]         = "%{wks.location}/ThirdParty/FreeType/include"
include_dir["SOL"]              = "%{wks.location}/ThirdParty/SOL/include"
include_dir["LUA"]              = "%{wks.location}/ThirdParty/LUA"

extern_lib_dir = {}
extern_lib_dir["SDL"]            = "%{wks.location}/ThirdParty/SDL/VisualC/x64/Debug"
extern_lib_dir["PHYSX"]          = "F:/PhysX/physx/bin/win.x86_64.vc143.mt/debug"
extern_lib_dir["PHYSX_RELEASE"]  = "F:/PhysX/physx/bin/win.x86_64.vc143.mt/release"
extern_lib_dir["ASSIMP"]         = "%{wks.location}/ThirdParty/ASSIMP/lib/Debug"
extern_lib_dir["GLFW"]           = "%{wks.location}/ThirdParty/GLFW/src/Debug"
extern_lib_dir["SteamSDK"]       = "%{wks.location}/ThirdParty/SteamSDK/redistributable_bin/win64"
extern_lib_dir["VULKAN"]         = "C:/VulkanSDK/1.4.309.0/Lib"
extern_lib_dir["FMOD"]           = "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64"
extern_lib_dir["FreeType"]       = "%{wks.location}/ThirdParty/FreeType/objs"

lib = {}
lib["Libs_Debug"]   = {
    "PhysX_64.lib", 
    "PhysXCharacterKinematic_static_64.lib", 
    "PhysXCommon_64.lib", 
    "PhysXCooking_64.lib", 
    "PhysXExtensions_static_64.lib", 
    "PhysXFoundation_64.lib", 
    "PhysXPvdSDK_static_64.lib", 
    "PhysXTask_static_64.lib", 
    "PhysXVehicle2_static_64.lib",
    --FMOD
    "fmodL_vc.lib",
    "freetype.lib",
    "Lua"
}
lib["Libs_Release"] = {
    "PhysX_64.lib", 
    "PhysXCharacterKinematic_static_64.lib", 
    "PhysXCommon_64.lib", 
    "PhysXCooking_64.lib", 
    "PhysXExtensions_static_64.lib", 
    "PhysXFoundation_64.lib", 
    "PhysXPvdSDK_static_64.lib", 
    "PhysXTask_static_64.lib", 
    "PhysXVehicle2_static_64.lib",
    --FMOD
    "fmod_vc.lib",
    "freetype.lib",
}

function SetupCommonProjectSettings()
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/obj/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        buildoptions "/MTd"
        links ( lib.Libs_Debug )
        libdirs { 
            "%{extern_lib_dir.PHYSX}",
            "%{extern_lib_dir.FMOD}",
            "%{extern_lib_dir.FreeType}",
        }
        defines{
            "DEBUG",
            "ENCRYPTION_XOR",
            "PLATFORM_WINDOWS",
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS",
            "PX_PHYSX_COOKING",
        }



    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        buildoptions "/MT"
        links ( lib.Libs_Release )
        libdirs { 
            "%{extern_lib_dir.PHYSX_RELEASE}" ,
            "%{extern_lib_dir.FMOD}",
            "%{extern_lib_dir.FreeType}",
        }
        defines{
            "RELEASE",
            "NDEBUG",
            "ENCRYPTION_XOR",
            "PLATFORM_WINDOWS",
            "WIN32_LEAN_AND_MEAN",
            "_CRT_SECURE_NO_WARNINGS",
            "PX_PHYSX_COOKING",
        }
        postbuildcommands{
            "copy %{wks.location}\\Kyrnness\\Game\\GameContent\\Game.kpak %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\GameContent\\Game.kpak",
            "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\release\\PhysX_64.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\PhysX_64.dll",
            "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\release\\PhysXCommon_64.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\PhysXCommon_64.dll",
            "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\release\\PhysXFoundation_64.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\PhysXFoundation_64.dll",
            "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\release\\PhysXCooking_64.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\PhysXCooking_64.dll",
            "copy %{wks.location}\\ThirdParty\\SDL\\VisualC\\x64\\Release\\SDL3.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\SDL3.dll",
            "copy %{wks.location}\\ThirdParty\\SteamSDK\\redistributable_bin\\win64\\steam_api64.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\steam_api64.dll",
            "copy %{wks.location}\\ThirdParty\\FMOD\\fmod.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\fmod.dll",
            "copy %{wks.location}\\ThirdParty\\FreeType\\objs\\freetype.dll %{wks.location}\\Build\\bin\\Release-windows-x86_64\\Game\\freetype.dll",
        }

    filter {    }
end

workspace "Kyrnness"
    architecture "x64"
    startproject "Game"

    configurations{ "Debug", "Release" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    assetFolder = "%{wks.location}/Kyrnness/Game/GameContent"

    flags
    {
        "MultiProcessorCompile"
    }

group "Programs"
   include "Kyrnness/Game"
   include "Kyrnness/Packer"
group ""
   
group "Libs"
   include "Kyrnness/KyrnnessCore"
   include "Kyrnness/KyrnnessSteamModule"
   include "Kyrnness/KyrnnessPacker"
group ""

group "ThirdParty"   
    include "ThirdParty/GLAD"
    include "ThirdParty/IMGUI"
    include "ThirdParty/ZLIB"
    include "ThirdParty/Lua"
group ""