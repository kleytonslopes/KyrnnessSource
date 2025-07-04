project "KyrnnessCore"
    kind "StaticLib"
    SetupCommonProjectSettings()

    pchheader "CorePCH.hpp"
    pchsource "Sources/CorePCH.cpp"

    files
    {
        "Sources/**.hpp",
        "Sources/**.cpp",
    }

    includedirs
    {
        "Sources",
        "%{include_dir.KyrnnessPacker}",
        "%{include_dir.FMOD}",
        "%{include_dir.SDL}",
        "%{include_dir.JSON}",
        "%{include_dir.GLAD}",
        "%{include_dir.ENTT}",
        "%{include_dir.GLM}",
        "%{include_dir.TINY}",
        "%{include_dir.TINYOBJ}",
        "%{include_dir.GLFW}",
        "%{include_dir.ASSIMP}",
        "%{include_dir.ASSIMP_BUILD}",
        "%{include_dir.STB}",
        "%{include_dir.IMGUI}",
        "%{include_dir.PHYSX}",
        "%{include_dir.SteamSDK}",
        "%{include_dir.VULKAN}",
        "%{include_dir.ZLIB}",
        "%{include_dir.FreeType}",
        "%{include_dir.SOL}",
        "%{include_dir.LUA}",
    }

    libdirs
    {
        "%{extern_lib_dir.SDL}",
        "%{extern_lib_dir.ASSIMP}",
        "%{extern_lib_dir.GLFW}",
        "%{extern_lib_dir.VULKAN}",
        "%{extern_lib_dir.FMOD}",
        
    }

    links
    {
        "GLAD",
        "IMGUI",
        "SDL3.lib",
        "vulkan-1.lib",
        "assimp-vc143-mtd.lib",
        "fmodL_vc.lib",
        "ZLIB",
        "Lua"
    }