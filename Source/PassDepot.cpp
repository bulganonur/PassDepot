#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

int main()
{
    if (!glfwInit())
    {
        return 1;
    }

    // Create window with graphics context
    GLFWwindow* Window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (Window == nullptr)
    {
        return 1;
    }
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& IO = ImGui::GetIO(); (void)IO;
    
    // Enable Keyboard Controls
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Enable Gamepad Controls
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init();

    // Our state
    bool bShowDemoWindow = true;
    bool bShowAnotherWindow = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(Window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the IO.WantCaptureMouse, IO.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When IO.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When IO.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (bShowDemoWindow)
        {
            ImGui::ShowDemoWindow(&bShowDemoWindow);
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int Counter = 0;

            // Create a window called "Hello, world!" and append into it.
            ImGui::Begin("Hello, world!");

            // Display some text (you can use a format strings too)
            ImGui::Text("This is some useful text.");

            // Edit bools storing our window open/close state
            ImGui::Checkbox("Demo Window", &bShowDemoWindow);
            ImGui::Checkbox("Another Window", &bShowAnotherWindow);

            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

            // Edit 3 floats representing a color
            ImGui::ColorEdit3("clear color", (float*)&clear_color);

            // Buttons return true when clicked (most widgets return true when edited/activated)
            if (ImGui::Button("Button"))
            {
                Counter++;
            }
            ImGui::SameLine();
            ImGui::Text("Counter = %d", Counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / IO.Framerate, IO.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (bShowAnotherWindow)
        {
            // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Begin("Another Window", &bShowAnotherWindow);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
            {
                bShowAnotherWindow = false;
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(Window);
    glfwTerminate();

    return 0;
}