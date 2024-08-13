#include "PDGUI.h"
#include "PDWindow.h"
#include "PDDatabase.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <iostream>
#include <sstream>

namespace PassDepot
{
 
PDGUI::PDGUI(PDWindow* InWindow, PDDatabase* InDatabase, PDSecurity* InSecurity)
: MainViewport(nullptr), bIsLoggedIn(false), bIsResizing(false), bIsTableHovered(false), bShouldShowStatusMsg(false), 
  bAreStatusPopupsON(true), TitleBarSizeY(40.0f), FontSize(16.0f), TitleBarButtonsSize{30.0f, 26.0f}, 
  StandardButtonSize{80.0f, 40.0f}, BigButtonSize{160.0f, 40.0f}, SmallButtonSize{40.0f, 35.0f}, 
  ResizeHandleSize{20.0f, 20.0f}, MouseDragDelta{0.0f, 0.0f}, OldWindowSize{0.0f, 0.0f}, StyleWindowPadding{10.0f, 10.0f}, 
  StyleItemSpacing{7.0f, 7.0f}, StyleItemInnerSpacing{4.0f, 4.0f}, StyleFramePadding{5.0f, 5.0f}, 
  ClearColor{0.0f, 0.0f, 0.0f, 0.0f}, StatusMessageColor{1.0f, 1.0f, 1.0f, 1.0f}, 
  TableFlags(
    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Hideable | 
    ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY),  
  MainWindowFlags(
    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar), 
  PopupMessageFlags(
    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | 
    ImGuiWindowFlags_NoSavedSettings), 
  TitleBarFlags(
    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse), 
  WelcomeOperation(EWO_Default), DepotOperation(EDO_Default), SelectedTheme(EPT_Default)
{
    Window = InWindow;
    Database = InDatabase;
    Security = InSecurity;
    Init();
}

PDGUI::~PDGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PDGUI::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& IO = ImGui::GetIO(); (void)IO;

    // Enable docking and // Enable Multi-Viewport/Platform Windows
    // IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Enable Keyboard Controls
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Enable Gamepad Controls
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Set fonts to VictorMono
    IO.Fonts->AddFontFromFileTTF("VictorMono-SemiBold.ttf", FontSize);

    SetupStyle();
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init();
    Window->SetWindowStartupPosition();
}

void PDGUI::SetupStyle()
{
    // Set startup Dear ImGui theme based on SelectedTheme
    SetSelectedTheme(EPT_Dark);
    SwitchThemes();

    ImGuiStyle& Style = ImGui::GetStyle();
    
    // Border sizes
    Style.WindowBorderSize = 1.0f;
    Style.FrameBorderSize = 1.0f;
    Style.ChildBorderSize = 1.0f;
    Style.PopupBorderSize = 1.0f;
    Style.TabBorderSize = 1.0f;
    Style.TabBarBorderSize = 2.0f;
    Style.SeparatorTextBorderSize = 1.0f;

    // Rounding
    Style.WindowRounding = 4.0f;
    Style.FrameRounding = 4.0f;
    Style.ChildRounding = 4.0f;
    Style.PopupRounding = 4.0f;
    Style.TabRounding = 4.0f;
    Style.GrabRounding = 4.0f;
    Style.ScrollbarRounding = 2.0f;

    // Padding
    Style.WindowPadding = StyleWindowPadding;
    Style.FramePadding = StyleFramePadding;
    Style.CellPadding = ImVec2{4.0f, 4.0f};
    Style.SeparatorTextPadding = ImVec2{0.0f, 0.0f};

    // Sizes
    Style.ScrollbarSize = 7.0f;
    Style.GrabMinSize = 14.0f;
    Style.TabBarOverlineSize = 2.0f;

    // Alignment
    Style.WindowTitleAlign = ImVec2{0.5f, 0.5f};
    Style.ButtonTextAlign = ImVec2{0.5f, 0.5f};
    Style.SeparatorTextAlign = ImVec2{0.5f, 0.5f};

    // Spacing
    Style.ItemSpacing = StyleItemSpacing;
    Style.ItemInnerSpacing = StyleItemInnerSpacing;
}

void PDGUI::OptionsButton()
{
    // Available themes
    const std::vector<std::string> Themes = { "Classic", "Dark", "Light" };
    std::vector<bool> Toggles = {false, false, false};
    
    Toggles[GetSelectedTheme()] = true;
    
    // Options Popup button
    AlignElementsX(TitleBarButtonsSize.x * 2.0f, 0.01f);
    if (ImGui::Button("Options", ImVec2{TitleBarButtonsSize.x * 2.0f, TitleBarButtonsSize.y}))
    {
        ImGui::OpenPopup("OptionsPopup");
    }

    // Popup contents
    if (ImGui::BeginPopup("OptionsPopup"))
    {
        // Select a theme
        ImGui::SeparatorText("Themes");
        for (int Idx = 0; Idx < Themes.size(); ++Idx)
        {
            if (ImGui::MenuItem(Themes[Idx].c_str(), "", Toggles[Idx], !Toggles[Idx]))
            {
                SetSelectedTheme(Idx);
            }
        }
        // Switch themes
        SwitchThemes();

        // Enable/disable popups
        ImGui::SeparatorText("Popups");
        ImGui::Checkbox("Messages", &bAreStatusPopupsON);

        // Logout
        if (IsLoggedIn())
        {
            ImGui::Separator();
            if (ImGui::Button("Logout"))
            {
                Logout();
            }
        }
        
        ImGui::EndPopup();
    }
}

void PDGUI::TitleBar()
{
    // Begin a new window as a custom title bar
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(Window->GetWindowSizeXF(), TitleBarSizeY));
    if (ImGui::Begin("TitleBar", nullptr, MainWindowFlags | ImGuiWindowFlags_NoScrollWithMouse))
    {
        // Options button
        OptionsButton();
        ImGui::SameLine();

        // Title text
        AlignElementsX(ImGui::CalcTextSize("PassDepot").x);
        ImGui::TextUnformatted("PassDepot");

        // MinimizeWindow button
        ImGui::SameLine();
        AlignElementsX(TitleBarButtonsSize.x, 0.99f, 3);
        if (ImGui::Button("-", TitleBarButtonsSize))
        {
            Window->MinimizeWindow();
        }
        // MaximizeWindow button
        ImGui::SameLine();
        if (ImGui::Button("[]", TitleBarButtonsSize))
        {
            Window->MaximizeWindow();
        }
        // CloseWindow button
        ImGui::SameLine();
        if (ImGui::Button("X", TitleBarButtonsSize))
        {
            Window->CloseWindow();
        }
    }
    ImGui::End();
}

void PDGUI::ResizeHandle()
{
    if (!Window->IsWindowMaximized())
    {
        // Resize handle upper left corner, min position
        ImVec2 ResizeHandleMin = ImVec2(Window->GetWindowSizeXF() - ResizeHandleSize.x, Window->GetWindowSizeYF() - ResizeHandleSize.y);

        // Adjust resize handle position for Style.WindowPadding
        ResizeHandleMin.x -= StyleWindowPadding.x;
        ResizeHandleMin.y -= StyleWindowPadding.y;

        // Resize handle bottom right, max position
        ImVec2 ResizeHandleMax = ImVec2(ResizeHandleMin.x + ResizeHandleSize.x, ResizeHandleMin.y + ResizeHandleSize.y);

        // Setup position, color and padding
        ImGui::SetNextWindowPos(ResizeHandleMin);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{0.1f, 0.1f, 0.5f, 0.5f});
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding | ImGuiStyleVar_FramePadding, ImVec2{0.0f, 0.0f});
        
        // Draw rectangle
        if (ImGui::BeginChild("ResizeHandle", ResizeHandleSize, 0, MainWindowFlags))
        {
            // Resize handle icon
            ImVec2 IconSize = ImGui::CalcTextSize("+");
            ImVec2 Center = {ResizeHandleMin.x + (ResizeHandleSize.x - IconSize.x) * 0.5f, ResizeHandleMin.y + (ResizeHandleSize.y - IconSize.y) * 0.5f};
            ImGui::SetCursorScreenPos(Center);
            ImGui::TextUnformatted("+");

            // Wants to resize
            if (ImGui::IsMouseHoveringRect(ResizeHandleMin, ResizeHandleMax) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                SetIsResizing(true);
                OldWindowSize = {Window->GetWindowSizeXF(), Window->GetWindowSizeYF()};
            }

            // Actual resizing
            if (IsResizing() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
                MouseDragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
                if (MouseDragDelta.x != 0.0f || MouseDragDelta.y != 0.0f)
                {
                    int NewSizeX = static_cast<int>(OldWindowSize.x + MouseDragDelta.x);
                    int NewSizeY = static_cast<int>(OldWindowSize.y + MouseDragDelta.y);

                    Window->SetWindowSize(NewSizeX, NewSizeY);
                }
            }
            else
            {
                SetIsResizing(false);
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
}

void PDGUI::WindowDragging()
{
    // Handle window dragging
    bool bShouldDrag = !Window->IsWindowMaximized() && !ImGui::IsKeyDown(ImGuiKey_LeftShift) && !IsResizing() && !IsTableHovered();
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && bShouldDrag)
    {
        MouseDragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
        if (MouseDragDelta.x != 0.0f || MouseDragDelta.y != 0.0f)
        {
            int NewPosX = Window->GetWindowPositionX() + static_cast<int>(MouseDragDelta.x);
            int NewPosY = Window->GetWindowPositionY() + static_cast<int>(MouseDragDelta.y);
            Window->SetWindowPosition(NewPosX, NewPosY);
        }
    }
}

void PDGUI::StartNewFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void PDGUI::MainWindows()
{
    MainViewport = ImGui::GetMainViewport();
    
    TitleBar();
    WindowDragging();
    StatusMessagePopup();

    if (IsLoggedIn())
    {
        Depot();
    }
    else
    {
        Welcome();
    }
}

void PDGUI::Render()
{
    ImGui::Render();
    // @todo: move these to somewhere appropriate
    int DisplayWidth, DisplayHeight;
    glfwGetFramebufferSize(Window->GetGLFWWindow(), &DisplayWidth, &DisplayHeight);
    glViewport(0, 0, DisplayWidth, DisplayHeight);
    glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows, this is for Multiviewport
    // GLFWwindow* CurrentContextBackup = glfwGetCurrentContext();
    // ImGui::UpdatePlatformWindows();
    // ImGui::RenderPlatformWindowsDefault();
    // glfwMakeContextCurrent(CurrentContextBackup);
    
    glfwSwapBuffers(Window->GetGLFWWindow());
}

void PDGUI::SetupStatusMessage(const std::string &InStatusMessage, const bool bIsErrorMessage)
{
    // if user has not explicitly disabled StatusMessagePopup()s, setup and popup messages
    if (AreStatusPopupsON() && !InStatusMessage.empty())
    {
        if (bIsErrorMessage)
        {
            // Red
            StatusMessageColor = {0.75f, 0.05f, 0.05f, 1.0f};
        }
        else
        {
            // Green
            StatusMessageColor = {0.05f, 0.75f, 0.05f, 1.0f};
        }

        SetStatusMessage(InStatusMessage);
        SetShouldShowStatusMsg(true);
    }
}

void PDGUI::StatusMessagePopup()
{
    if (ShouldShowStatusMsg())
    {
        ImGui::SetNextWindowPos(MainViewport->GetCenter(), ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});
        if (ImGui::Begin("StatusMessage", nullptr, PopupMessageFlags))
        {
            if (!ImGui::IsWindowFocused())
            {
                SetShouldShowStatusMsg(false);
            }
            ImGui::Spacing();
            ImGui::TextColored(StatusMessageColor, StatusMessage.c_str());
            ImGui::Spacing();
        }
        ImGui::End();
    }
}

void PDGUI::Welcome()
{
    ImGui::SetNextWindowPos(ImVec2(MainViewport->Pos.x, MainViewport->Pos.y + TitleBarSizeY));
    ImGui::SetNextWindowSize(ImVec2(Window->GetWindowSizeXF(), Window->GetWindowSizeYF() - TitleBarSizeY));
    if (ImGui::Begin("Welcome", nullptr, MainWindowFlags))
    {   
        // Dummy spacer for style
        ImGui::Dummy(ImVec2{0.0f, ImGui::GetWindowHeight() * 0.20f});

        SeparatorWithNewLines();

        // Welcome text
        AlignElementsX(ImGui::CalcTextSize("Login or Register to PassDepot:").x);
        ImGui::TextUnformatted("Login or Register to PassDepot:");
        
        ImGui::Spacing();
        
        // Radio buttons
        AlignElementsX(ImGui::CalcTextSize("Login Register").x * 0.5f, 0.5f, 2, 2);
        ImGui::RadioButton("Login", &WelcomeOperation, EWO_Login);
        ImGui::SameLine();
        ImGui::RadioButton("Register", &WelcomeOperation, EWO_Register);

        SeparatorWithNewLines();

        if (GetWelcomeOperation() == EWO_Login)
        {
            Login();
        }
        else if (GetWelcomeOperation() == EWO_Register)
        {
            Register();
        }

        ResizeHandle();
    }
    ImGui::End();   
}

void PDGUI::Login()
{
    float InputFieldWidth = MainViewport->Size.x * 0.5f;
    
    // Input fields
    InputFieldsLogin(InputFieldWidth);

    ImGui::Spacing();

    // Login button and authentication
    AlignElementsX(StandardButtonSize.x);
    if (ImGui::Button("LOGIN", StandardButtonSize))
    {
        if (std::strlen(DepotUsername) <= 0 || std::strlen(MasterPassword) <= 0)
        {
            SetupStatusMessage("Must Provide a Username and a Password", true);
        }
        else if (!Database->DoesUsernameExists(DepotUsername))
        {
            SetupStatusMessage("Wrong Username or Password", true);
        }
        else if (!Security->VerifyPassword(Database->GetHash(DepotUsername), MasterPassword))
        {
            SetupStatusMessage("Wrong Username or Password", true);
        }
        else
        {
            // Setup user related data(key, userid), load depot and login
            ProcessLogin();
        }
    }
}

void PDGUI::Register()
{
    float InputFieldWidth = MainViewport->Size.x * 0.5f;

    // Input fields
    InputFieldsRegister(InputFieldWidth);

    ImGui::Spacing();

    // Register
    AlignElementsX(StandardButtonSize.x);
    if (ImGui::Button("REGISTER", StandardButtonSize))
    {
        if (std::strlen(DepotUsername) <= 0 || std::strlen(MasterPassword) <= 0)
        {
            SetupStatusMessage("Must Provide a Username and a Password", true);
        }
        else if (std::strcmp(MasterPassword, MasterPassVerify) != 0)
        {
            SetupStatusMessage("Passwords Do Not Match", true);
        }
        else if (Database->DoesUsernameExists(DepotUsername))
        {
            SetupStatusMessage("Invalid Username", true);
        }
        else
        {
            // Register user
            Database->InsertDataUserTable(DepotUsername, Security->HashPassword(MasterPassword), Security->GenerateSalt());
            
            // Setup user related data(key, userid), load depot and login
            ProcessLogin();
        }
    }
}

void PDGUI::ProcessLogin()
{
    // Derive a key that will be used for encryption/decryption
    Security->DeriveKey(MasterPassword, Database->GetSalt(DepotUsername));

    // Mark user, so appropriate data is loaded from the database
    Database->CacheUserId(DepotUsername);

    // Load Depot
    LoadAndMakeDepotDisplayReady();

    // Clear input fields
    ClearInputFieldsWelcome();

    // Set the view to no operation, just table view
    SetDepotOperation(EDO_NOOP);

    // Login
    SetIsLoggedIn(true);
}

void PDGUI::Depot()
{
    ImGui::SetNextWindowPos(ImVec2(MainViewport->Pos.x, MainViewport->Pos.y + TitleBarSizeY));
    ImGui::SetNextWindowSize(ImVec2(Window->GetWindowSizeXF(), Window->GetWindowSizeYF() - TitleBarSizeY));
    if (ImGui::Begin("Depot", nullptr, MainWindowFlags))
    {
        // Depot Table 
        if (ImGui::BeginTable("DepotTable", 5, TableFlags, ImVec2{MainViewport->Size.x - StyleWindowPadding.x * 2.0f, MainViewport->Size.y * 0.5f}))
        {
            // Headers
            ImGui::TableSetupColumn("EntryId");
            ImGui::TableSetupColumn("Title");
            ImGui::TableSetupColumn("Username");
            ImGui::TableSetupColumn("Password");
            ImGui::TableSetupColumn("Notes");
            ImGui::TableHeadersRow();

            // Table Contents
            for (const DisplayEntry& DisplayEntry : DisplayDepot)
            {   
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                std::string Id = std::to_string(DisplayEntry.EntryId);
                ImGui::TextUnformatted(Id.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(DisplayEntry.Title.c_str());
                
                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(DisplayEntry.Username.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::TextUnformatted(DisplayEntry.Password.c_str());

                ImGui::TableSetColumnIndex(4);
                ImGui::TextWrapped(DisplayEntry.Notes.c_str());
            }
            
            if (ImGui::TableGetHoveredColumn() == -1)
            {
                SetIsTableHovered(false);
            }
            else
            {
                SetIsTableHovered(true);
            }

            ImGui::EndTable();
        }

        SeparatorWithNewLines();

        /**
         * -------------------------------------------------------------------------------------------------------------
         * New Entry, Edit Entry, Delete Entry
         * -------------------------------------------------------------------------------------------------------------
         */
        
        // Radio buttons
        AlignElementsX(ImGui::CalcTextSize("New Entry Edit Entry Delete Entry Just Depot").x * 0.25f, 0.5f, 4, 4);
        
        ImGui::RadioButton("New Entry", &DepotOperation, EDO_NewEntry);
        ImGui::SameLine();
        ImGui::RadioButton("Edit Entry", &DepotOperation, EDO_EditEntry);
        ImGui::SameLine();
        ImGui::RadioButton("Delete Entry", &DepotOperation, EDO_DeleteEntry);
        ImGui::SameLine();
        ImGui::RadioButton("Just Depot", &DepotOperation, EDO_NOOP);

        ImGui::NewLine();

        if (GetDepotOperation() == EDO_NewEntry)
        {
            AddNewEntry();
            ImGui::NewLine();
        }
        else if (GetDepotOperation() == EDO_EditEntry)
        {
            EditEntry();
            ImGui::NewLine();
        }
        else if (GetDepotOperation() == EDO_DeleteEntry)
        {
            DeleteEntry();
            ImGui::NewLine();
        }
        else
        {
            // NOOP, just Depot table view
        }
        
        ImGui::Separator();
        
        ResizeHandle();
    }
    ImGui::End();
}

void PDGUI::AddNewEntry()
{
    float InputFieldWidth = MainViewport->Size.x * 0.5f;

    // Input fields
    InputFieldsAddNewEntry(InputFieldWidth);
    
    ImGui::Spacing();

    AlignElementsX(BigButtonSize.x, 0.5f, 2);
    if (ImGui::Button("Save New Entry", BigButtonSize))
    {
        PrepareAndRecordEntry();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", BigButtonSize))
    {
        // Clear input fields
        ClearInputFieldsDepot();
    }
}

void PDGUI::EditEntry()
{
    float InputFieldWidth = MainViewport->Size.x * 0.5f;

    // Input fields
    InputFieldsEditEntry(InputFieldWidth);

    ImGui::Spacing();

    AlignElementsX(BigButtonSize.x, 0.5f, 3);
    if (ImGui::Button("Save Changes", BigButtonSize))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetupStatusMessage("No Such Entry, Enter a Valid EntryId", true);
        }
        else
        {
            PrepareAndRecordEntry();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Fill in Details", BigButtonSize))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetupStatusMessage("No Such Entry, Enter a Valid EntryId", true);
        }
        else
        {
            // Get requested entry
            DisplayEntry Entry = GetDisplayEntry(EntryId);
            
            // Fill input fields with the details
            std::strncpy(Title, Entry.Title.c_str(), sizeof(Title));
            std::strncpy(Username, Entry.Username.c_str(), sizeof(Username));
            std::strncpy(Password, Entry.Password.c_str(), sizeof(Password));
            std::strncpy(Notes, Entry.Notes.c_str(), sizeof(Notes));
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", BigButtonSize))
    {
        // Clear input fields
        ClearInputFieldsDepot();
    }
}

void PDGUI::DeleteEntry()
{
    float InputFieldWidth = MainViewport->Size.x * 0.5f;

    // Input fields
    InputFieldsDeleteEntry(InputFieldWidth);

    ImGui::Spacing();

    AlignElementsX(BigButtonSize.x);
    if (ImGui::Button("Delete", BigButtonSize))
    {
        if (!Database->DoesEntryExists(EntryId))
        {
            SetupStatusMessage("No Such Entry, Enter a Valid EntryId", true);
        }
        else
        {
            // Pop-up warning Are you sure
            ImGui::OpenPopup("PopupWarning");
        }
    }

    // Are you sure pop-up modal
    ImGui::SetNextWindowPos(MainViewport->GetCenter(), ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{0.85f, 0.1f, 0.1f, 0.7f});
    if (ImGui::BeginPopupModal("PopupWarning", nullptr, PopupMessageFlags))
    {
        ImGui::Spacing();

        // Format text beforehand so its size will be calculated correctly
        std::string WarningText = fmt::format("Entry No:{} will be deleted permanently.", EntryId);
        ImGui::TextUnformatted(WarningText.c_str());

        ImGui::Spacing();

        ImGui::SeparatorText("Are you sure?");

        ImGui::NewLine();

        // Center upcoming buttons
        float TextWidth = ImGui::CalcTextSize(WarningText.c_str()).x;
        float ButtonsWidth = (StandardButtonSize.x * 2.0f) + StyleItemSpacing.x;
        float PopupCenter = (TextWidth - ButtonsWidth) * 0.5f;
        ImVec2 CursorScreenPos = ImGui::GetCursorScreenPos();
        ImVec2 IndentSize = {CursorScreenPos.x + PopupCenter, CursorScreenPos.y};
        
        ImGui::SetCursorScreenPos(IndentSize);
        
        if (ImGui::Button("Delete", StandardButtonSize))
        {
            Database->DeleteDataDepotTable(EntryId);
            LoadAndMakeDepotDisplayReady();
            SetupStatusMessage("Entry Deleted Successfully", false);
            ClearInputFieldsDepot();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", StandardButtonSize))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
}

void PDGUI::PrepareAndRecordEntry()
{
    // Create EntryInfo
    const std::string EntryInfo = CreateEntryInfo(Title, Username, Password, Notes);
    
    // Concatenate user input
    std::string PlainText = Title;
    PlainText.append(Username);
    PlainText.append(Password);
    PlainText.append(Notes);

    // return early if all fields are empty
    if (PlainText.empty())
    {
        return;
    }
    
    // Encrypt PlainText and convert it to hex, size is plaintext + Cipher Tag
    std::vector<unsigned char> CipherText(crypto_secretbox_MACBYTES + PlainText.length());
    const std::vector<unsigned char> Nonce = Security->Encrypt(CipherText, PlainText, Security->GetKey());
    const std::string CipherHex = Security->BinToHex(CipherText);
    const std::string NonceHex = Security->BinToHex(Nonce);

    if (GetDepotOperation() == EDO_NewEntry)
    {
        // Insert new entry into database and show message
        Database->InsertDataDepotTable(CipherHex, NonceHex, EntryInfo);
        SetupStatusMessage("Entry Recorded Successfully", false);
    }
    else if (GetDepotOperation() == EDO_EditEntry)
    {
        // Update entry and show message
        Database->UpdateDataDepotTable(EntryId, CipherHex, NonceHex, EntryInfo);
        SetupStatusMessage("Entry Updated Successfully", false);
    }

    // Reload Depot
    LoadAndMakeDepotDisplayReady();

    // Clear input fields
    ClearInputFieldsDepot();
}

void PDGUI::Logout()
{
    Security->ClearKey();
    Database->SetUserId(0);
    ClearInputFieldsDepot();
    SetWelcomeOperation(EWO_Default);
    SetIsLoggedIn(false);
}

void PDGUI::InputFieldsLogin(float InFieldWidth)
{
    ImGui::PushItemWidth(InFieldWidth);
    
    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("##DepotUsername", "Username", DepotUsername, IM_ARRAYSIZE(DepotUsername));
    
    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("##MasterPassword", "MasterPassword", MasterPassword, IM_ARRAYSIZE(MasterPassword), ImGuiInputTextFlags_Password);

    ImGui::PopItemWidth();
}

void PDGUI::InputFieldsRegister(float InFieldWidth)
{
    InputFieldsLogin(InFieldWidth);
    
    ImGui::PushItemWidth(InFieldWidth);

    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("##MasterPassVerify", "Re-enter MasterPassword", MasterPassVerify, IM_ARRAYSIZE(MasterPassVerify), ImGuiInputTextFlags_Password);

    ImGui::PopItemWidth();
}

void PDGUI::InputFieldsAddNewEntry(float InFieldWidth)
{
    ImGui::PushItemWidth(InFieldWidth);

    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("Title", "Title", Title, IM_ARRAYSIZE(Title));

    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("Username", "Username", Username, IM_ARRAYSIZE(Username));

    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("Password", "Password", Password, IM_ARRAYSIZE(Password));

    AlignElementsX(InFieldWidth);
    ImGui::InputTextWithHint("Notes", "Notes", Notes, IM_ARRAYSIZE(Notes));

    ImGui::PopItemWidth();
}

void PDGUI::InputFieldsEditEntry(float InFieldWidth)
{
    InputFieldsDeleteEntry(InFieldWidth);
    InputFieldsAddNewEntry(InFieldWidth);
}

void PDGUI::InputFieldsDeleteEntry(float InFieldWidth)
{
    ImGui::PushItemWidth(InFieldWidth);

    AlignElementsX(InFieldWidth);
    ImGui::InputInt("EntryId", &EntryId);

    ImGui::PopItemWidth();
}

std::string PDGUI::CreateEntryInfo(const char *InTitle, const char *InUsername, const char *InPassword, const char *InNotes)
{
    std::string EntryInfo = std::to_string(std::strlen(InTitle)) + ".";
    EntryInfo.append(std::to_string(std::strlen(InUsername)) + ".");
    EntryInfo.append(std::to_string(std::strlen(InPassword)) + ".");
    EntryInfo.append(std::to_string(std::strlen(InNotes)));
    
    return EntryInfo;
}

std::vector<std::string> PDGUI::MakeEntryDisplayReady(const DepotEntry& InEntry)
{
    // Ready EntryInfo and calculate Plaintext length
    std::vector<int> EntryInfo;
    int PlainTextLength = 0;
    std::stringstream StrStream(InEntry.EntryInfo);
    std::string Buffer;
    while (std::getline(StrStream, Buffer, '.'))
    {
        EntryInfo.push_back(std::stoi(Buffer));
        PlainTextLength += std::stoi(Buffer);
    }

    // Decrypt ciphertext
    std::vector<unsigned char> PlainBytes(PlainTextLength + 1);
    Security->Decrypt(PlainBytes, InEntry.CipherText, Database->GetNonce(InEntry.EntryId), Security->GetKey());
    std::string PlainText(PlainBytes.begin(), PlainBytes.end());
    
    // Divide Plaintext into substrings, each will be a column in the Depot table
    size_t StartPosition = 0;
    std::vector<std::string> ColumnValues;
    for (const int Length : EntryInfo)
    {
        ColumnValues.push_back(PlainText.substr(StartPosition, Length));
        StartPosition += Length;
    }

    return ColumnValues;
}

void PDGUI::LoadAndMakeDepotDisplayReady()
{
    DisplayDepot.clear();
    Database->LoadDepot();
    const std::vector<DepotEntry>& Depot = Database->GetDepot();

    // Populate display depot
    for (const DepotEntry& Entry : Depot)
    {
        std::vector<std::string> ColumnValues = MakeEntryDisplayReady(Entry);
        DisplayEntry PrettyEntry
        (
            Entry.EntryId, 
            ColumnValues[0], 
            ColumnValues[1], 
            ColumnValues[2], 
            ColumnValues[3]
        );

        DisplayDepot.push_back(PrettyEntry);
    }
}

DisplayEntry PDGUI::GetDisplayEntry(int InEntryId) const
{
    for (const DisplayEntry& Entry : DisplayDepot)
    {
        if (Entry.EntryId == InEntryId)
        {
            return Entry;
        }
    }
    return DisplayEntry();
}

void PDGUI::ClearInputFieldsWelcome()
{
    std::fill(std::begin(DepotUsername), std::end(DepotUsername), '\0');
    std::fill(std::begin(MasterPassword), std::end(MasterPassword), '\0');
    std::fill(std::begin(MasterPassVerify), std::end(MasterPassVerify), '\0');
}

void PDGUI::ClearInputFieldsDepot()
{
    EntryId = 0;
    std::fill(std::begin(Title), std::end(Title), '\0');
    std::fill(std::begin(Username), std::end(Username), '\0');
    std::fill(std::begin(Password), std::end(Password), '\0');
    std::fill(std::begin(Notes), std::end(Notes), '\0');
}

void PDGUI::SwitchThemes()
{
    if (GetSelectedTheme() == EPT_Classic)
    {
        ImGui::StyleColorsClassic();
    }
    else if (GetSelectedTheme() == EPT_Dark)
    {
        ImGui::StyleColorsDark();
    }
    else
    {
        ImGui::StyleColorsLight();
    }
}

void PDGUI::SeparatorWithNewLines()
{
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
}

void PDGUI::AlignElementsX(float InElementWidth, float InAlignment, int InElementCount, int InRadioButtonCount)
{
    float ItemSpacingTotal = (InElementCount - 1) * StyleItemSpacing.x;
    float ElementWidthTotal = InElementCount * InElementWidth + ItemSpacingTotal;

    if (InRadioButtonCount != 0)
    { 
        float ButtonsWidthTotal = (FontSize + StyleFramePadding.y * 2 - 1) * InRadioButtonCount;
        ElementWidthTotal += ButtonsWidthTotal;
    }

    float IndentSize = (MainViewport->Size.x - ElementWidthTotal) * InAlignment;
    ImGui::SetCursorScreenPos(ImVec2{IndentSize, ImGui::GetCursorScreenPos().y});
}

} // namespace PassDepot