#include "PassDepot.h"

#include <iostream>
#include <stdexcept>

// #define PASSDEPOT_NO_CONSOLE

#if defined(_WIN32) && defined(PASSDEPOT_NO_CONSOLE)
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    PassDepot::PassDepot PassDepot{};
    try
    {
        PassDepot.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
#else
int main()
{
    PassDepot::PassDepot PassDepot{};
    try
    {
        PassDepot.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
#endif