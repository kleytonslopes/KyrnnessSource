#include "DebugUtils.hpp"

#ifdef _DEBUG
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <locale.h>

void OpenDebugConsole()
{
    AllocConsole();

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    printf("Debug Console Aberta!\n");
}
#else
void OpenDebugConsole() {}
#endif