#pragma once

#include <string>

struct GLFWwindow;

namespace PassDepot
{
    
class PDWindow
{

public:

    PDWindow(int Width, int Height, std::string Name);
    ~PDWindow();

    // delete copy ctor and copy assignment operator to avoid copies
    PDWindow(const PDWindow&) = delete;
    PDWindow& operator=(const PDWindow&) = delete;

public:

    inline GLFWwindow* GetGLFWWindow() const { return Window; }
    bool ShouldClose() const;

private:

    void InitWindow();

    // Window Properties
    GLFWwindow* Window;
    int WindowWidth;
    int WindowHeight;
    std::string WindowName;

};

} // namespace PassDepot
