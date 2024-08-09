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

    void PollEvents() const;

    void CloseWindow() const;
    bool ShouldClose() const;
    void MaximizeWindow(); 
    void MinimizeWindow() const;

    void GetWindowPositionAndSize();
    void SetWindowPosition(int InPositionX, int InPositionY);
    void SetWindowSize(int InSizeX, int InSizeY);
    void SetWindowStartupPosition();

    inline GLFWwindow* GetGLFWWindow() const { return Window; }

    inline int GetWindowPositionX() const { return WindowPositionX; }
    inline int GetWindowPositionY() const { return WindowPositionY; }
    inline int GetWindowSizeX() const { return WindowSizeX; }
    inline int GetWindowSizeY() const { return WindowSizeY; }

    inline float GetWindowPositionXF() const { return static_cast<float>(WindowPositionX); }
    inline float GetWindowPositionYF() const { return static_cast<float>(WindowPositionY); }
    inline float GetWindowSizeXF() const { return static_cast<float>(WindowSizeX); }
    inline float GetWindowSizeYF() const { return static_cast<float>(WindowSizeY); }

    inline bool IsWindowMaximized() const { return bIsWindowMaximized; }

    inline int GetVideoModeWidth() const { return VideoModeWidth; }
    inline int GetVideoModeHeight() const { return VideoModeHeight; }

    inline int GetWindowSizeMinX() const { return WindowSizeMinX; }
    inline int GetWindowSizeMinY() const { return WindowSizeMinY; }

private:

    void InitWindow();
    void CacheVideoModeWidthHeight();

    // Window Properties
    GLFWwindow* Window;
    int WindowPositionX;
    int WindowPositionY;
    int WindowSizeX;
    int WindowSizeY;
    int WindowSizeMinX;
    int WindowSizeMinY;
    std::string WindowName;
    bool bIsWindowMaximized;

    int VideoModeWidth;
    int VideoModeHeight;
};

} // namespace PassDepot
