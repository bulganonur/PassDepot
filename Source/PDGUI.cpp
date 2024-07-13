#include "PDGUI.h"
#include "PDDatabase.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


namespace PassDepot
{
    
PDGUI::PDGUI(GLFWwindow* Window) : bLoggedIn(false), bShowDepot(false), ClearColor{ImVec4(0.45f, 0.55f, 0.60f, 1.00f)}
{
    Init(Window);
}

PDGUI::~PDGUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PDGUI::Init(GLFWwindow* Window)
{
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
}

void PDGUI::StartNewFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void PDGUI::Render(GLFWwindow *Window)
{
    // Rendering
    ImGui::Render();
    int DisplayWidth, DisplayHeight;
    glfwGetFramebufferSize(Window, &DisplayWidth, &DisplayHeight);
    glViewport(0, 0, DisplayWidth, DisplayHeight);
    glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(Window);
}

void PDGUI::DemoDemo()
{
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
            ImGui::ColorEdit3("clear color", (float*)&ClearColor);

            // Buttons return true when clicked (most widgets return true when edited/activated)
            if (ImGui::Button("Button"))
            {
                Counter++;
            }
            ImGui::SameLine();
            ImGui::Text("Counter = %d", Counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
}

void PDGUI::Welcome()
{
    if (!bLoggedIn)
    {    
        ImGui::Begin("Welcome");

        ImGui::Text("Login or Register to PassDepot");

        
        static char Username[32] = "";
        ImGui::InputTextWithHint("##", "Username", Username, IM_ARRAYSIZE(Username));
        
        static char MasterPassword[128] = "";
        ImGui::InputTextWithHint("###", "MasterPassword", MasterPassword, IM_ARRAYSIZE(MasterPassword), ImGuiInputTextFlags_Password);
        
        // Login Authentication
        if (ImGui::Button("Login"))
        {
            if (strcmp(MasterPassword, "hello") == 0)
            {
                ImGui::Text("YES");
                bLoggedIn = true;
                bShowDepot = true;
            }
            else
            {
                ImGui::Text("NO");
            }
        }

        ImGui::SameLine();

        // Register new user
        if (ImGui::Button("Register"))
        {
            if (strlen(Username) <= 0)
            {
                ImGui::Text("Invalid Username");
                
            }
            else
            {
                // register to database
            }
            
            

            ImGui::Text("YES");
            bLoggedIn = true;
            bShowDepot = true;
            
        }
        ImGui::End();   
    }
}

void PDGUI::Depot()
{
    if (bShowDepot)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 800), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Example: Simple layout", nullptr, ImGuiWindowFlags_MenuBar))
        {
            
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Close", "Ctrl+W")) { bShowDepot = false; }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Left
            static int selected = 0;
            {
                ImGui::BeginChild("left pane", ImVec2(400, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
                for (int i = 0; i < 100; i++)
                {
                    // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                    char label[128];
                    sprintf_s(label, "MyObject %d", i);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();

            // Right
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                ImGui::Text("MyObject: %d", selected);
                ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    if (ImGui::BeginTabItem("Description"))
                    {
                        ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Details"))
                    {
                        ImGui::Text("ID: 0123456789");
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::EndChild();
                if (ImGui::Button("Revert")) {}
                ImGui::SameLine();
                if (ImGui::Button("Save")) {}
                ImGui::EndGroup();
            }
        }
        ImGui::End();
    }
    
}

} // namespace PassDepot


