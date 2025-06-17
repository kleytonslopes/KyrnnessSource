project "KyrnnessSteamModule"
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
        "%{include_dir.SteamSDK}",

    }

    libdirs
    {

    }

    links
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
                "APP_EDITOR_MODE"
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
                "APP_GAME_MODE"
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
            }
            buildoptions "/MT"
            optimize "on"