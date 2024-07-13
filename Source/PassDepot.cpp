#include "PassDepot.h"

#include <GLFW/glfw3.h>
#include <sodium.h>

namespace PassDepot
{

PassDepot::PassDepot() : Window{1280, 720, "PassDepot"}, GUI{Window.GetGLFWWindow()}, Database{}
{
    
}

PassDepot::~PassDepot()
{

}

void PassDepot::Run()
{
    while (!Window.ShouldClose())
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the IO.WantCaptureMouse, IO.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When IO.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When IO.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        GUI.StartNewFrame();

        GUI.DemoDemo();
        GUI.Welcome();
        GUI.Depot();

        GUI.Render(Window.GetGLFWWindow());
    }
}

} // namespace PassDepot

