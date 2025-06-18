project "Game"
    kind "WindowedApp"
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
        "copy %{wks.location}\\ThirdParty\\SDL\\VisualC\\x64\\Debug\\SDL3.dll $(SolutionDir)Kyrnness\\Game\\SDL3.dll",
        "C:\\VulkanSDK\\1.4.309.0\\Bin\\glslangValidator -V %{wks.location}\\Assets\\Shaders\\shader.vert -o %{wks.location}\\Assets\\Shaders\\shader.vert.spv",
        "C:\\VulkanSDK\\1.4.309.0\\Bin\\glslangValidator -V %{wks.location}\\Assets\\Shaders\\shader.frag -o %{wks.location}\\Assets\\Shaders\\shader.frag.spv",
        "copy %{wks.location}\\Assets\\Shaders\\shader.frag.spv $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\shader.frag.spv",
        "copy %{wks.location}\\Assets\\Shaders\\shader.vert.spv $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\shader.vert.spv",

        "copy %{wks.location}\\Assets\\Shaders\\OpenGL\\vert.glsl $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\OpenGL\\vert.glsl",
        "copy %{wks.location}\\Assets\\Shaders\\OpenGL\\frag.glsl $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\OpenGL\\frag.glsl",
        "copy %{wks.location}\\Assets\\Shaders\\OpenGL\\debug_vert.glsl $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\OpenGL\\debug_vert.glsl",
        "copy %{wks.location}\\Assets\\Shaders\\OpenGL\\debug_frag.glsl $(SolutionDir)Kyrnness\\Game\\Assets\\Shaders\\OpenGL\\debug_frag.glsl",

        "copy %{wks.location}\\Assets\\Models\\gizmo.glb $(SolutionDir)Kyrnness\\Game\\Assets\\Models\\gizmo.glb",
        "copy %{wks.location}\\Assets\\Models\\SM_Floor.glb $(SolutionDir)Kyrnness\\Game\\Assets\\Models\\SM_Floor.glb",
        "copy %{wks.location}\\Assets\\Models\\SM_Wall.glb $(SolutionDir)Kyrnness\\Game\\Assets\\Models\\SM_Wall.glb",
        "copy %{wks.location}\\ThirdParty\\ASSIMP\\bin\\Debug\\assimp-vc143-mtd.dll $(SolutionDir)Kyrnness\\Game\\assimp-vc143-mtd.dll",
        "copy %{wks.location}\\ThirdParty\\SteamSDK\\redistributable_bin\\win64\\steam_api64.dll $(SolutionDir)Kyrnness\\Game\\steam_api64.dll",

        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysX_64.dll $(SolutionDir)Kyrnness\\Game\\PhysX_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXCommon_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXCommon_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXFoundation_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXFoundation_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXCooking_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXCooking_64.dll"
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