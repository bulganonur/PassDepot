#pragma once

#include <imgui.h>

#include <string>

struct GLFWwindow;

namespace PassDepot
{

class PDDatabase;

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
	void Depot();

	// @todo(debugcode): remove
	void DemoDemo();

	// ImGui elements should end before this function
	void Render(GLFWwindow* Window);

	// Shows status' of various operations, like a pop-up
	void StatusWindow();

private:

	void Init(GLFWwindow* Window);
	void SetStatusMessage(const std::string& InStatusMessage);

	bool bLoggedIn;
	bool bShowDepot;
	bool bShowStatusWindow;
	std::string StatusMessage;
    ImVec4 ClearColor;
	
	// @todo(debugcode): remove
    bool bShowDemoWindow = true;
    bool bShowAnotherWindow = false;

	
	/**
	 * bLoggedIn = false;
	 * if db found and not empty
	 * 		while (!Login())
	 * 			;
	 * 		PasswordStorageScreen();
	 * else
	 * 		Register();
	 * 		PasswordStorageScreen();
	 * 
	 */
};

	
} // namespace PassDepot