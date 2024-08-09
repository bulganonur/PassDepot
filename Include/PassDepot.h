#pragma once

#include "PDWindow.h"
#include "PDDatabase.h"
#include "PDSecurity.h"
#include "PDGUI.h"

namespace PassDepot
{
    
class PassDepot
{

public:

    PassDepot();
    ~PassDepot();

    PassDepot(const PassDepot&) = delete;
    PassDepot& operator=(const PassDepot&) = delete;

public:

    void Run();

private:

    PDWindow Window;
    PDDatabase Database;
    PDSecurity Security;
    PDGUI GUI;
};


} // namespace PassDepot
