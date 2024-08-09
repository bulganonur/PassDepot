#include "PassDepot.h"

namespace PassDepot
{

PassDepot::PassDepot() : Window{1280, 720, "PassDepot"}, Database{}, Security{}, GUI{&Window, &Database, &Security}
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
        Window.PollEvents();

        // Start Dear Imgui frame
        GUI.StartNewFrame();

        // Get glfw window size and position
        Window.GetWindowPositionAndSize();

        // All of the gui elements
        GUI.MainWindows();
        
        // Render elements
        GUI.Render();
    }
}

} // namespace PassDepot

