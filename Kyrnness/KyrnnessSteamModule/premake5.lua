project "KyrnnessSteamModule"
    kind "StaticLib"
    SetupCommonProjectSettings()

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

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