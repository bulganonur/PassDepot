#include "PDWindow.h"

#include <GLFW/glfw3.h>

namespace PassDepot
{

PDWindow::PDWindow(int Width, int Height, std::string Name) : WindowWidth{Width}, WindowHeight{Height}, WindowName{Name}
{
    InitWindow();
}

PDWindow::~PDWindow()
{
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void PDWindow::InitWindow()
{
    glfwInit();
    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1); // Enable vsync
}

bool PDWindow::ShouldClose() const
{
    return glfwWindowShouldClose(Window);
}
    
} // namespace PassDepot


