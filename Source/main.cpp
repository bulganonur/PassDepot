#include "PassDepot.h"

#include <iostream>
#include <stdexcept>

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