#include "pch.hpp"
#include "Runtime/Packer.hpp"
#include "aes.h"

int main()
{
    fs::path inputDir   = "../../Kyrnness/Game/GameContent";
    fs::path outputFile = "../../Kyrnness/Game/GameContent/Game.kpak";

    UPacker::PackDirectory(inputDir, outputFile);

    return 0;
}