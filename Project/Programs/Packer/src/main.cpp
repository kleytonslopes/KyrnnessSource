#include "pch.hpp"
#include "Runtime/Packer.hpp"
#include "aes.h"

int main()
{
    fs::path inputDir   = "C:/Projects/KyrnnessSource/Project/Programs/Game/GameContent";
    fs::path outputFile = "C:/Projects/KyrnnessSource/Project/Programs/Game/GameContent/Game.kpak";

    UPacker::PackDirectory(inputDir, outputFile);

    return 0;
}