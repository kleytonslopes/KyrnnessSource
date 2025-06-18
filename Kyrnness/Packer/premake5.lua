project "Packer"
    kind "ConsoleApp"
    SetupCommonProjectSettings()

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
        "%{include_dir.KyrnnessCore}",
        "%{include_dir.KyrnnessPacker}",
        "%{include_dir.KyrnnessSteamModule}",
        "%{include_dir.KyrnnessAudio}",
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
        "%{include_dir.FMOD}",
        "%{include_dir.ZLIB}",
    }

    libdirs
    {
        "%{extern_lib_dir.SDL}",
        "%{extern_lib_dir.ASSIMP}",
        "%{extern_lib_dir.GLFW}",
        "%{extern_lib_dir.SteamSDK}",
        "%{extern_lib_dir.VULKAN}",
    }

    links
    {
        "opengl32.lib",
        "fmodL_vc.lib",
        "GLAD",
        "IMGUI",
        "ZLIB",
        "KyrnnessAudio",
        "KyrnnessCore",
        "KyrnnessPacker",
        "KyrnnessSteamModule",
        "SDL3.lib",
        "vulkan-1.lib",
        "assimp-vc143-mtd.lib",
        "glfw3.lib",
        "steam_api64.lib",
    }

    postbuildcommands
    {
        
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