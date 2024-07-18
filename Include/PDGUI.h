#pragma once

#include <imgui.h>

#include <string>

struct GLFWwindow;

namespace PassDepot
{

class PDDatabase;

enum WelcomeOperations
{
	WO_Register, 
	WO_Login, 
	WO_Default
};

enum DepotOperations
{
	DO_NewEntry, 
	DO_EditEntry, 
	DO_DeleteEntry, 
	DO_NOOP, 
	DO_Default
};

/**
 * GUI Manager that handles Dear Imgui elements 
 */
class PDGUI
{

public:

	PDGUI(GLFWwindow* Window);
	~PDGUI();

	// delete copy ctor and copy assignment operator to prevent copies
	PDGUI(const PDGUI&) = delete;
	PDGUI& operator=(const PDGUI&) = delete;

public:

	// ImGui elements should start after this function
	void StartNewFrame();

	// Returns true if login/register is successful
	void Welcome(PDDatabase* Database);
	void Depot(PDDatabase* Database);

	// @todo(debugcode): remove
	void DemoDemo();

	// ImGui elements should end before this function
	void Render(GLFWwindow* Window);

	// Shows status' of various operations, like a pop-up
	void StatusWindow();

private:

	void Init(GLFWwindow* Window);
	void SetStatusMessage(const std::string& InStatusMessage);
	void AddNewEntry(PDDatabase* Database);
	void EditEntry(PDDatabase* Database);
	void DeleteEntry(PDDatabase* Database);
	void Login(PDDatabase* Database);
	void Register(PDDatabase* Database);
	
	bool bLoggedIn;
	bool bShowDepot;
	bool bShowStatusWindow;
	std::string StatusMessage;
    ImVec4 ClearColor;
	
	int DepotOperation;
	int WelcomeOperation;

	// @todo(debugcode): remove
    bool bShowDemoWindow = true;
    bool bShowAnotherWindow = false;

};

	
} // namespace PassDepot