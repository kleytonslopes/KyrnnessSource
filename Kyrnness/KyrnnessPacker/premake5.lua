project "KyrnnessPacker"
    kind "StaticLib"
    SetupCommonProjectSettings()

    -- pchheader "pch.hpp"
    -- pchsource "src/pch.cpp"


    files
    {
        "include/**.hpp",
        "include/**.c",
        "include/**.h",
        "src/**.cpp",
        "src/**.c",
        "src/**.h"

    }

    includedirs
    {
        "include",
        "%{include_dir.ZLIB}",
    }

    libdirs
    {

    }

    links
    {
       "ZLIB",
    }