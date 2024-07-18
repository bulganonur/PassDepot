#include "PDGUI.h"
#include "PDDatabase.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

namespace PassDepot
{
    
PDGUI::PDGUI(GLFWwindow* Window)
: bLoggedIn(false), bShowDepot(false), bShowStatusWindow(false), ClearColor{ImVec4(0.45f, 0.55f, 0.60f, 1.00f)}, 
  DepotOperation(DO_Default), WelcomeOperation(WO_Default)
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

void PDGUI::SetStatusMessage(const std::string &InStatusMessage)
{
    StatusMessage = InStatusMessage;
    bShowStatusWindow = true;
}

void PDGUI::AddNewEntry(PDDatabase* Database)
{
    static char Title[64] = "";
    ImGui::InputTextWithHint("Title", "Title", Title, IM_ARRAYSIZE(Title));
    
    static char Username[64] = "";
    ImGui::InputTextWithHint("Username", "Username", Username, IM_ARRAYSIZE(Username));

    static char Password[64] = "";
    ImGui::InputTextWithHint("Password", "Password", Password, IM_ARRAYSIZE(Password));

    static char Notes[256] = "";
    ImGui::InputTextWithHint("Notes", "Notes", Notes, IM_ARRAYSIZE(Notes));

    if (ImGui::Button("Save New Entry", ImVec2(200.0f, 50.0f)))
    {
        Database->InsertDataDepotTable(Title, Username, Password, Notes);
        Database->LoadDepot();
        SetStatusMessage("Entry Recorded Successfully");

        strncpy_s(Title, "", sizeof(Title));
        strncpy_s(Username, "", sizeof(Username));
        strncpy_s(Password, "", sizeof(Password));
        strncpy_s(Notes, "", sizeof(Notes));
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(200.0f, 50.0f)))
    {
        strncpy_s(Title, "", sizeof(Title));
        strncpy_s(Username, "", sizeof(Username));
        strncpy_s(Password, "", sizeof(Password));
        strncpy_s(Notes, "", sizeof(Notes));
    }
}

void PDGUI::EditEntry(PDDatabase *Database)
{
    static int EntryId;
    ImGui::InputInt("EntryId", &EntryId);

    static char Title[64] = "";
    ImGui::InputTextWithHint("Title", "Title", Title, IM_ARRAYSIZE(Title));
    
    static char Username[64] = "";
    ImGui::InputTextWithHint("Username", "Username", Username, IM_ARRAYSIZE(Username));

    static char Password[64] = "";
    ImGui::InputTextWithHint("Password", "Password", Password, IM_ARRAYSIZE(Password));

    static char Notes[256] = "";
    ImGui::InputTextWithHint("Notes", "Notes", Notes, IM_ARRAYSIZE(Notes));

    if (ImGui::Button("Save Changes", ImVec2(200.0f, 50.0f)))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetStatusMessage("No Such Entry, Enter a Valid EntryId");
        }
        else
        {
            Database->UpdateDataDepotTable(EntryId, Title, Username, Password, Notes);
            Database->LoadDepot();
            SetStatusMessage("Entry Recorded Successfully");

            EntryId = 0;
            strncpy_s(Title, "", sizeof(Title));
            strncpy_s(Username, "", sizeof(Username));
            strncpy_s(Password, "", sizeof(Password));
            strncpy_s(Notes, "", sizeof(Notes));
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Fill in Details", ImVec2(200.0f, 50.0f)))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetStatusMessage("No Such Entry, Enter a Valid EntryId");
        }
        else
        {
            DepotEntry Entry = Database->GetEntry(EntryId);
            
            strncpy_s(Title, Entry.Title.c_str(), sizeof(Title));
            strncpy_s(Username, Entry.Username.c_str(), sizeof(Username));
            strncpy_s(Password, Entry.Password.c_str(), sizeof(Password));
            strncpy_s(Notes, Entry.Notes.c_str(), sizeof(Notes));
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(200.0f, 50.0f)))
    {
        EntryId = 0;
        strncpy_s(Title, "", sizeof(Title));
        strncpy_s(Username, "", sizeof(Username));
        strncpy_s(Password, "", sizeof(Password));
        strncpy_s(Notes, "", sizeof(Notes));
    }
}

void PDGUI::DeleteEntry(PDDatabase *Database)
{
    static int EntryId;
    ImGui::InputInt("EntryId", &EntryId);

    if (ImGui::Button("Delete Entry", ImVec2(200.0f, 50.0f)))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetStatusMessage("No Such Entry, Enter a Valid EntryId");
        }
        else
        {
            Database->DeleteDataDepotTable(EntryId);
            Database->LoadDepot();
            SetStatusMessage("Entry Deleted Successfully");
        }
    }
}

void PDGUI::Login(PDDatabase *Database)
{
    static char Username[32] = "";
    ImGui::InputTextWithHint("##", "Username", Username, IM_ARRAYSIZE(Username));
    
    static char MasterPassword[128] = "";
    ImGui::InputTextWithHint("###", "MasterPassword", MasterPassword, IM_ARRAYSIZE(MasterPassword), ImGuiInputTextFlags_Password);
    
    // Login Authentication
    if (ImGui::Button("LOGIN"))
    {
        if (strlen(Username) <= 0 || strlen(MasterPassword) <= 0)
        {
            SetStatusMessage("Must Provide a Username and a Password");
        }
        else if (!Database->Authenticate(Username, MasterPassword))
        {
            SetStatusMessage("Wrong Username or Password");
        }
        else
        {
            Database->SetUserId(Username);
            Database->LoadDepot();
            bShowStatusWindow = false;
            bLoggedIn = true;
            bShowDepot = true;
        }
    }
}

void PDGUI::Register(PDDatabase *Database)
{
    static char Username[32] = "";
    ImGui::InputTextWithHint("##", "Username", Username, IM_ARRAYSIZE(Username));
    
    static char MasterPassword[128] = "";
    ImGui::InputTextWithHint("###", "MasterPassword", MasterPassword, IM_ARRAYSIZE(MasterPassword), ImGuiInputTextFlags_Password);

    static char MasterPassVerify[128] = "";
    ImGui::InputTextWithHint(" ", "Re-enter MasterPassword", MasterPassVerify, IM_ARRAYSIZE(MasterPassVerify), ImGuiInputTextFlags_Password);

    if (ImGui::Button("REGISTER"))
    {
        if (strlen(Username) <= 0 || strlen(MasterPassword) <= 0)
        {
            SetStatusMessage("Must Provide a Username and a Password");
        }
        else if (strcmp(MasterPassword, MasterPassVerify) != 0)
        {
            SetStatusMessage("Passwords Do Not Match");
        }
        else if (Database->DoesUsernameExists(Username))
        {
            SetStatusMessage("Invalid Username");
        }
        else
        {
            // Register to database
            Database->InsertDataUserTable(Username, MasterPassword);
            Database->SetUserId(Username);
            Database->LoadDepot();

            SetStatusMessage("Registered Successfully");

            bLoggedIn = true;
            bShowDepot = true;
        }
    }
}

void PDGUI::StatusWindow()
{
    // @todo: Implement "ShowStatusMesseges?" functionality

    if (bShowStatusWindow)
    {
        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Begin("Message", &bShowStatusWindow);
        ImGui::TextWrapped(StatusMessage.c_str());
        ImGui::Spacing();
        if (ImGui::Button("Close"))
        {
            bShowStatusWindow = false;
        }
        ImGui::End();
    }
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

void PDGUI::Welcome(PDDatabase* Database)
{
    if (!bLoggedIn)
    {    
        ImGui::Begin("Welcome");
        ImGui::Text("Login or Register to PassDepot: ");
        ImGui::Spacing();

        ImGui::RadioButton("Login", &WelcomeOperation, WO_Login);
        ImGui::SameLine();
        ImGui::RadioButton("Register", &WelcomeOperation, WO_Register);

        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        if (WelcomeOperation == WO_Login)
        {
            Login(Database);
        }
        else if (WelcomeOperation == WO_Register)
        {
            Register(Database);
        }
        ImGui::End();   
    }
}

void PDGUI::Depot(PDDatabase* Database)
{
    if (bShowDepot)
    {
        if (ImGui::Begin("PassDepot"))
        {
            // Table flags
            static ImGuiTableFlags Flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable;
            
            // Depot Table 
            if (ImGui::BeginTable("Depot", 5, Flags))
            {
                // Headers
                ImGui::TableSetupColumn("EntryId");
                ImGui::TableSetupColumn("Title");
                ImGui::TableSetupColumn("Username");
                ImGui::TableSetupColumn("Password");
                ImGui::TableSetupColumn("Notes");
                ImGui::TableHeadersRow();

                // Table Contents
                for (DepotEntry& DepotEntry : *Database->GetDepot())
                {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    std::string EntryId = std::to_string(DepotEntry.EntryId);
                    ImGui::TextUnformatted(EntryId.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(DepotEntry.Title.c_str());
                    
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextUnformatted(DepotEntry.Username.c_str());

                    ImGui::TableSetColumnIndex(3);
                    ImGui::TextUnformatted(DepotEntry.Password.c_str());

                    ImGui::TableSetColumnIndex(4);
                    ImGui::TextWrapped(DepotEntry.Notes.c_str());
                }
            }
            ImGui::EndTable();

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();

            /**
             * ---------------------------------------------------------------------------------------------------------
             * New Entry, Edit Entry, Delete Entry
             * ---------------------------------------------------------------------------------------------------------
             */

            ImGui::RadioButton("New Entry", &DepotOperation, DO_NewEntry);
            ImGui::SameLine();
            ImGui::RadioButton("Edit Entry", &DepotOperation, DO_EditEntry);
            ImGui::SameLine();
            ImGui::RadioButton("Delete Entry", &DepotOperation, DO_DeleteEntry);
            ImGui::SameLine();
            ImGui::RadioButton("Just Depot", &DepotOperation, DO_NOOP);

            ImGui::NewLine();

            if (DepotOperation == DO_NewEntry)
            {
                AddNewEntry(Database);
            }
            else if (DepotOperation == DO_EditEntry)
            {
                EditEntry(Database);
            }
            else if (DepotOperation == DO_DeleteEntry)
            {
                DeleteEntry(Database);
            }
            else
            {
                // NOOP, just Depot table view
            }
            
            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();
            
            if (ImGui::Button("Log Out", ImVec2(200.0f, 100.0f)))
            {
                bShowDepot = false;
                bLoggedIn = false;
            }
        }
        ImGui::End();
    }
}

} // namespace PassDepot
