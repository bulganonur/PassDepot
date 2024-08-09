#pragma once

#include "PDSecurity.h"

#include <imgui.h>

#include <string>
#include <vector>

namespace PassDepot
{

class PDWindow;
class PDDatabase;
struct DepotEntry;


/**
 * Holds entries that have been made display ready
 */
struct DisplayEntry
{
	DisplayEntry(int InEntryId, const std::string& InTitle, const std::string& InUsername, const std::string& InPassword, const std::string& InNotes)
	: EntryId(InEntryId), Title(InTitle), Username(InUsername), Password(InPassword), Notes(InNotes)
	{}

	DisplayEntry() {}

	int EntryId;
	std::string Title;
	std::string Username;
	std::string Password;
	std::string Notes;
};

/**
 * GUI Manager that handles Dear Imgui elements
 * This class is the starting point for most of the operations
 * Contains PDWindow, PDDatabase and PDSecurity
 * 
 * Aside from TitleBar() and StatusMessagePopup() there are two ImGui::Begin() functions, in Welcome() and in Depot(). 
 * Both of them are called in MainWindows() but only one of them is rendered at a time. 
 * Welcome() branches into Login() and Register(), whereas Depot() into AddNewEntry(), EditEntry() and DeleteEntry().
 */
class PDGUI
{

public:

	PDGUI(PDWindow* InWindow, PDDatabase* InDatabase, PDSecurity* InSecurity);
	~PDGUI();

	// delete copy ctor and copy assignment operator to prevent copies
	PDGUI(const PDGUI&) = delete;
	PDGUI& operator=(const PDGUI&) = delete;

public:

	// ImGui elements should start after this function
	void StartNewFrame();

	// Based on the bIsLoggedIn status, either shows a Login/Register window or the Depot itself
	void MainWindows();

	// ImGui elements should end before this function
	void Render();

private:

	// Initialization, called from the ctor
	void Init();

	// Setup Dear Imgui Style
	void SetupStyle();

	// Let the user select from one of the available themes
	void OptionsButton();

	// A window used as a custom title bar
	void TitleBar();

	// Handles window resizing
	void ResizeHandle();

	// Handles window dragging
	void WindowDragging();

	// Sets the contents and color of the StatusMessagePopup
	void SetupStatusMessage(const std::string& InStatusMessage, bool bIsErrorMessage);

	// Shows status' of various operations, like a pop-up
	void StatusMessagePopup();

	void Welcome();
	void Login();
    void Register();
    void ProcessLogin();

    void Depot();
    void AddNewEntry();
	void EditEntry();
	void DeleteEntry();
	void PrepareAndRecordEntry();
	void Logout();

private:

	void InputFieldsLogin(float InFieldWidth);
	void InputFieldsRegister(float InFieldWidth);
	void InputFieldsAddNewEntry(float InFieldWidth);
	void InputFieldsEditEntry(float InFieldWidth);
	void InputFieldsDeleteEntry(float InFieldWidth);

	std::string CreateEntryInfo(const char* InTitle, const char* InUsername, const char* InPassword, const char* InNotes);
	std::vector<std::string> MakeEntryDisplayReady(const DepotEntry& InEntry);
	void LoadAndMakeDepotDisplayReady();
	DisplayEntry GetDisplayEntry(int InEntryId) const;
	
	void ClearInputFieldsWelcome();
	void ClearInputFieldsDepot();
	
	void SwitchThemes();
	void SeparatorWithNewLines();

	// Aligns a single element or multiple elements concatenated with Imgui::Sameline().
	// Uses default Style.ItemSpacing to calculate total item width, so if custom spacing is set for Imgui::Sameline()
	// alignment will be off
	void AlignElementsX(float InElementWidth, float InAlignment = 0.5f, int InElementCount = 1, int InRadioButtonCount = 0);
	
private:

	PDWindow* Window;
	PDDatabase* Database;
	PDSecurity* Security;

	// Login/Register buffers
	char DepotUsername[crypto_pwhash_STRBYTES] = {0};
	char MasterPassword[crypto_pwhash_STRBYTES] = {0};
	char MasterPassVerify[crypto_pwhash_STRBYTES] = {0};

	// New/Edit/Delete Entry buffers
	int EntryId = 0;
	char Title[crypto_pwhash_STRBYTES] = {0};
	char Username[crypto_pwhash_STRBYTES] = {0};
	char Password[crypto_pwhash_STRBYTES] = {0};
	char Notes[crypto_pwhash_STRBYTES * 2] = {0};

	// Depot that holds the display ready entries
	std::vector<DisplayEntry> DisplayDepot;

	// Message of the status window pop-up
	std::string StatusMessage;

	// Mainly used for getting viewport size and position
	ImGuiViewport* MainViewport;

	bool bIsLoggedIn;
	bool bIsResizing;
	bool bIsTableHovered;
	bool bShouldShowStatusMsg;
	bool bAreStatusPopupsON;
	
	float TitleBarSizeY;
	float FontSize;
	ImVec2 TitleBarButtonsSize;
	ImVec2 StandardButtonSize;
	ImVec2 BigButtonSize;
	ImVec2 SmallButtonSize;
	ImVec2 ResizeHandleSize;
	ImVec2 MouseDragDelta;
    ImVec2 OldWindowSize;
	ImVec2 StyleWindowPadding;
	ImVec2 StyleItemSpacing;
	ImVec2 StyleItemInnerSpacing;
	ImVec2 StyleFramePadding;

    ImVec4 ClearColor;
	ImVec4 StatusMessageColor;

	ImGuiTableFlags TableFlags;
	ImGuiWindowFlags MainWindowFlags;
	ImGuiWindowFlags PopupMessageFlags;
	ImGuiWindowFlags TitleBarFlags;
	
private:

	// Welcome window radio buttons
	enum EWelcomeOperations
	{
		EWO_Register, 
		EWO_Login, 
		EWO_Default
	};

	// Depot window radio buttons
	enum EDepotOperations
	{
		EDO_NewEntry, 
		EDO_EditEntry, 
		EDO_DeleteEntry, 
		EDO_NOOP, 
		EDO_Default
	};

	// Available themes from Dear ImGui
	enum EPassDepotThemes
	{
		EPT_Classic, 
		EPT_Dark , 
		EPT_Light, 
		EPT_Default
	};

	int WelcomeOperation;
	int DepotOperation;
	int SelectedTheme;

private:

	inline bool IsLoggedIn() const { return bIsLoggedIn; }
	inline bool IsResizing() const { return bIsResizing; }
	inline bool IsTableHovered() const { return bIsTableHovered; }
	inline bool ShouldShowStatusMsg() const { return bShouldShowStatusMsg; }
	inline int GetDepotOperation() const { return DepotOperation; }
	inline int GetWelcomeOperation() const { return WelcomeOperation; }
	inline int GetSelectedTheme() const { return SelectedTheme; }
	inline std::string GetStatusMessage() const { return StatusMessage; }
	inline bool AreStatusPopupsON() const { return bAreStatusPopupsON; }

	inline void SetIsLoggedIn(bool InbIsLoggedIn) { bIsLoggedIn = InbIsLoggedIn; }
	inline void SetIsResizing(bool InbIsResizing) { bIsResizing = InbIsResizing; }
	inline void SetIsTableHovered(bool InbIsTableHovered) { bIsTableHovered = InbIsTableHovered; }
	inline void SetShouldShowStatusMsg(bool InbShouldShowStatusMsg) { bShouldShowStatusMsg = InbShouldShowStatusMsg; }
	inline void SetDepotOperation(int InDepotOperation) { DepotOperation = InDepotOperation; }
	inline void SetWelcomeOperation(int InWelcomeOperation) { WelcomeOperation = InWelcomeOperation; }
	inline void SetSelectedTheme(int InEPassDepotThemes) { SelectedTheme = InEPassDepotThemes; }
	inline void SetStatusMessage(const std::string& InStatusMessage) { StatusMessage = InStatusMessage; }
	inline void SetAreStatusPopupsON(bool InbAreStatusPopupsON) { bAreStatusPopupsON = InbAreStatusPopupsON; }

};

} // namespace PassDepot