#include "PDWindow.h"

#include <GLFW/glfw3.h>

#include <algorithm>

namespace PassDepot
{

PDWindow::PDWindow(int Width, int Height, std::string Name)
: WindowSizeX(Width), WindowSizeY(Height), WindowName{Name}, bIsWindowMaximized(false), WindowPositionX(0), WindowPositionY(0), 
  VideoModeWidth(0), VideoModeHeight(0), WindowSizeMinX(360), WindowSizeMinY(360)
{
    InitWindow();
    CacheVideoModeWidthHeight();
}

PDWindow::~PDWindow()
{
    glfwDestroyWindow(Window);
    glfwTerminate();
}

void PDWindow::InitWindow()
{
    glfwInit();

    // Disable decorations, mainly title bar, resizing
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Enable transparent framebuffer
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    // Create window and set it current context
    Window = glfwCreateWindow(WindowSizeX, WindowSizeY, WindowName.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(Window);
    
    // Enable vsync
    glfwSwapInterval(1);
}

void PDWindow::CacheVideoModeWidthHeight()
{
    GLFWmonitor* PrimaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* VideoMode = glfwGetVideoMode(PrimaryMonitor);
    VideoModeWidth = VideoMode->width;
    VideoModeHeight = VideoMode->height;
}

void PDWindow::PollEvents() const
{
    glfwPollEvents();
}

void PDWindow::CloseWindow() const
{
    glfwSetWindowShouldClose(Window, GLFW_TRUE);
}

bool PDWindow::ShouldClose() const
{
    return glfwWindowShouldClose(Window);
}

void PDWindow::MaximizeWindow()
{
    if (bIsWindowMaximized)
    {
        glfwRestoreWindow(Window);
        bIsWindowMaximized = false;
    }
    else
    {
        glfwMaximizeWindow(Window);
        bIsWindowMaximized = true;
    }
}

void PDWindow::MinimizeWindow() const
{
    glfwIconifyWindow(Window);
}

void PDWindow::GetWindowPositionAndSize()
{
    glfwGetWindowPos(Window, &WindowPositionX, &WindowPositionY);
    glfwGetWindowSize(Window, &WindowSizeX, &WindowSizeY);
}

void PDWindow::SetWindowPosition(int InPositionX, int InPositionY)
{
    glfwSetWindowPos(Window, InPositionX, InPositionY);
}

void PDWindow::SetWindowSize(int InSizeX, int InSizeY)
{
    // Clamp to minimum/maximum size
    int NewSizeX = std::clamp(InSizeX, GetWindowSizeMinX(), GetVideoModeWidth());
    int NewSizeY = std::clamp(InSizeY, GetWindowSizeMinY(), GetVideoModeHeight());

    glfwSetWindowSize(Window, NewSizeX, NewSizeY);
}

void PDWindow::SetWindowStartupPosition()
{
    int StartupWidth = static_cast<int>((VideoModeWidth - WindowSizeX) * 0.5f);
    int StartupHeight = static_cast<int>((VideoModeHeight - WindowSizeY) * 0.5f);

    SetWindowPosition(StartupWidth, StartupHeight);
}

} // namespace PassDepot
