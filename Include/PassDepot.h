#pragma once

#include "PDWindow.h"
#include "PDGUI.h"
#include "PDDatabase.h"

namespace PassDepot
{
    
class PassDepot
{

public:

    PassDepot();
    ~PassDepot();

public:

    void Run();

private:

    PDWindow Window;
    PDGUI GUI;
    PDDatabase Database;
};


} // namespace PassDepot
