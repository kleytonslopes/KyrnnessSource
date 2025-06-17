project "KyrnnessCore"
    kind "StaticLib"
    language "C++"
    cppdialect "c++20"
    staticruntime "on"

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    targetdir ("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Build/obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.hpp",
        "include/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "include",
        -- "generated",
        -- "%{wks.location}/Engine/Kyrnness/include",
        -- "%{wks.location}/Engine/Kyrnness/generated",
        "%{include_dir.KyrnnessPacker}",
        "%{include_dir.SDL}",
        "%{include_dir.JSON}",
        "%{include_dir.GLAD}",
        "%{include_dir.ENTT}",
        -- "%{include_dir.UUID}",
        "%{include_dir.GLM}",
        "%{include_dir.TINY}",
        "%{include_dir.TINYOBJ}",
        "%{include_dir.GLFW}",
        -- "%{include_dir.YAML}",
        "%{include_dir.ASSIMP}",
        "%{include_dir.ASSIMP_BUILD}",
        "%{include_dir.STB}",
        -- "%{include_dir.MINI}",
        -- "%{include_dir.IMGUI}",
        "%{include_dir.PHYSX}",
        -- "%{include_dir.KyrnnessMath}",
        "%{include_dir.SteamSDK}",
        "%{include_dir.VULKAN}",
    }

    libdirs
    {
        "%{extern_lib_dir.SDL}",
        "%{extern_lib_dir.ASSIMP}",
        -- "%{extern_lib_dir.PHYSX}",
        -- "%{extern_lib_dir.SteamSDK}",
        "%{extern_lib_dir.VULKAN}",
    }

    links
    {
        -- "opengl32.lib",
        -- "Kyrnness",
        "GLAD",
        -- "IMGUI",
        -- "YAML",
        -- "KyrnnessMath",
        "SDL3.lib",
        "vulkan-1.lib",
        "assimp-vc143-mtd.lib",
        -- "steam_api64.lib",
        -- "volk.lib",
        -- "PhysX_64.lib",
        -- "PhysXCharacterKinematic_static_64.lib",
        -- "PhysXCommon_64.lib",
        -- "PhysXCooking_64.lib",
        -- "PhysXExtensions_static_64.lib",
        -- "PhysXFoundation_64.lib",
        -- "PhysXPvdSDK_static_64.lib",
        -- "PhysXTask_static_64.lib",
        -- "PhysXVehicle_static_64.lib",
        -- "PhysXVehicle2_static_64.lib",
        -- "PVDRuntime_64.lib",
        -- "SceneQuery_static_64.lib",
        -- "SimulationController_static_64.lib",
    }

    filter "system:windows"
        systemversion "latest"

        filter "configurations:Debug"
            defines 
            {
                "DEBUG",
                "PLATFORM_WINDOWS",
                "WIN32_LEAN_AND_MEAN",
                "_CRT_SECURE_NO_WARNINGS",
                "YAML_CPP_STATIC_DEFINE",
                "LOG_WARNING",
                "LOG_INFORMATION",
                "LOG_DEBUG",
                "LOG_TRACE",
                "LOG_DEFAULT",
                "PX_PHYSX_COOKING"
            }
            buildoptions "/MTd" --MDd
            symbols "on"

        filter "configurations:EditorDebug"
            defines 
            {
                "DEBUG",
                "PLATFORM_WINDOWS",
                "WIN32_LEAN_AND_MEAN",
                "_CRT_SECURE_NO_WARNINGS",
                "YAML_CPP_STATIC_DEFINE",
                "LOG_WARNING",
                "LOG_INFORMATION",
                "LOG_DEBUG",
                "LOG_TRACE",
                "LOG_DEFAULT",
                "APP_EDITOR_MODE",
                "PX_PHYSX_COOKING"
            }
            buildoptions "/MTd"
            symbols "on"

        filter "configurations:GameDebug"
            defines 
            {
                "DEBUG",
                "PLATFORM_WINDOWS",
                "WIN32_LEAN_AND_MEAN",
                "_CRT_SECURE_NO_WARNINGS",
                "YAML_CPP_STATIC_DEFINE",
                "LOG_WARNING",
                "LOG_INFORMATION",
                "LOG_DEBUG",
                "LOG_TRACE",
                "LOG_DEFAULT",
                "APP_GAME_MODE",
                "PX_PHYSX_COOKING"
            }
            buildoptions "/MDd"
            symbols "on"

        filter "configurations:Release"
            defines 
            {
                "NDEBUG",
                "PLATFORM_WINDOWS",
                "WIN32_LEAN_AND_MEAN",
                "_CRT_SECURE_NO_WARNINGS",
                "YAML_CPP_STATIC_DEFINE",
                "PX_PHYSX_COOKING"
            }
            buildoptions "/MT"
            optimize "on"