#pragma once

#include <imgui.h>

struct GLFWwindow;

namespace PassDepot
{

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

	void StartNewFrame();
	void Welcome();
	void Depot();
	void DemoDemo();
	void Render(GLFWwindow* Window);

private:

	void Init(GLFWwindow* Window);

	bool bLoggedIn;
	bool bShowDepot;
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