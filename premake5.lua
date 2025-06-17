include_dir = {}
include_dir["KyrnnessCore"]        = "%{wks.location}/Kyrnness/KyrnnessCore/include"
include_dir["KyrnnessPacker"]      = "%{wks.location}/Kyrnness/KyrnnessPacker/include"
include_dir["KyrnnessSteamModule"] = "%{wks.location}/Kyrnness/KyrnnessSteamModule/include"
include_dir["KyrnnessAudio"]       = "%{wks.location}/Kyrnness/KyrnnessAudio/include"

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
--C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core\inc

extern_lib_dir = {}
extern_lib_dir["SDL"]      = "%{wks.location}/ThirdParty/SDL/VisualC/x64/Debug"
extern_lib_dir["PHYSX"]    = "F:/PhysX/physx/bin/win.x86_64.vc143.mt/debug"
extern_lib_dir["ASSIMP"]   = "%{wks.location}/ThirdParty/ASSIMP/lib/Debug"
extern_lib_dir["GLFW"]     = "%{wks.location}/ThirdParty/GLFW/src/Debug"
extern_lib_dir["SteamSDK"] = "%{wks.location}/ThirdParty/SteamSDK/redistributable_bin/win64"
extern_lib_dir["VULKAN"]   = "C:/VulkanSDK/1.4.309.0/Lib"
extern_lib_dir["FMOD"]     = "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64"


workspace "Kyrnness"
    architecture "x64"
    startproject "Game"

    configurations{ "Debug", "EditorDebug", "GameDebug", "Release" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    flags
    {
        "MultiProcessorCompile"
    }

    group "Programs"
   include "Kyrnness/Game"
group ""

group "Libs"
    include "Kyrnness/KyrnnessCore"
    include "Kyrnness/KyrnnessPacker"
    include "Kyrnness/KyrnnessSteamModule"
    include "Kyrnness/KyrnnessAudio"
group ""

group "ThirdParty"   
    include "ThirdParty/GLAD"
    include "ThirdParty/IMGUI"
group ""