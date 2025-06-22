project "Game"
    kind "WindowedApp"
    SetupCommonProjectSettings()

    files
    {
        "include/**.hpp",
        "include/**.h",
        "src/**.cpp",
        "version.rc",
        "gcrw_ico.ico"
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
        "%{include_dir.FreeType}",
        "%{include_dir.SOL}",
        "%{include_dir.LUA}",
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
        "copy %{wks.location}\\Kyrnness\\Game\\GameContent\\Game.kpak %{wks.location}\\Build\\bin\\Debug-windows-x86_64\\Game\\GameContent\\Game.kpak",
        "copy %{wks.location}\\Kyrnness\\Game\\GameContent\\Game.kpak %{wks.location}\\Kyrnness\\Game\\GameContent\\Game.kpak",

        "copy %{wks.location}\\ThirdParty\\SDL\\VisualC\\x64\\Debug\\SDL3.dll $(SolutionDir)Kyrnness\\Game\\SDL3.dll",
        "copy %{wks.location}\\ThirdParty\\ASSIMP\\bin\\Debug\\assimp-vc143-mtd.dll $(SolutionDir)Kyrnness\\Game\\assimp-vc143-mtd.dll",
        "copy %{wks.location}\\ThirdParty\\SteamSDK\\redistributable_bin\\win64\\steam_api64.dll $(SolutionDir)Kyrnness\\Game\\steam_api64.dll",

        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysX_64.dll $(SolutionDir)Kyrnness\\Game\\PhysX_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXCommon_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXCommon_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXFoundation_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXFoundation_64.dll",
        "copy F:\\PhysX\\physx\\bin\\win.x86_64.vc143.mt\\debug\\PhysXCooking_64.dll $(SolutionDir)Kyrnness\\Game\\PhysXCooking_64.dll",

        "copy %{wks.location}\\ThirdParty\\FreeType\\objs\\freetype.dll $(SolutionDir)Kyrnness\\Game\\freetype.dll",
    }
