#pragma once

#include <string>
#include <vector>

struct sqlite3;

namespace PassDepot
{

struct DepotEntry
{
    DepotEntry(int InEntryId, const unsigned char* InTitle, const unsigned char* InUsername, const unsigned char* InPassword, const unsigned char* InNotes, int InUserId)
    {
        EntryId = InEntryId;
        Title = (char*)InTitle;
        Username = (char*)InUsername;
        Password = (char*)InPassword;
        Notes = (char*)InNotes;
        UserId = InUserId;
    }

    DepotEntry(){};

    int EntryId;
    std::string Title;
    std::string Username;
    std::string Password;
    std::string Notes;
    int UserId;
};

/**
 * Manager class that handles sqlite3 operations
 */
class PDDatabase
{

public:

    PDDatabase();
    ~PDDatabase();

    // delete copy ctor and copy assignment operator so it can not be copied
    PDDatabase(const PDDatabase&) = delete;
    PDDatabase& operator=(const PDDatabase&) = delete;

public:

    void InsertDataUserTable(const std::string& Username, const std::string& MasterPassword);
    void InsertDataDepotTable(const std::string& Title, const std::string& Username, const std::string& Password, const std::string& Notes);
    void UpdateDataDepotTable(const int EntryId, const std::string& Title, const std::string& Username, const std::string& Password, const std::string& Notes);
    void DeleteDataDepotTable(const int EntryId);

    // Load Depot table into memory
    void LoadDepot();

    // Query database for the Username given
    bool DoesUsernameExists(const std::string& Username);

    // Query database for the Title given
    bool DoesEntryExists(const int EntryId);

    // Authentication for the login process
    bool Authenticate(const std::string& Username, const std::string& MasterPassword);

    // Getter for the Depot
    std::vector<DepotEntry>* GetDepot() { return &Depot; }

    // Gets UserId for the currently logged in user and caches it, so that data related to only that user is processed 
    void SetUserId(const std::string& Username);

    // Get a specific row from database
    DepotEntry GetEntry(const int EntryId);
    

private:

    void InitDatabaseWithTables();
    
    sqlite3* SqliteDatabase;
    char* ErrorMessage = nullptr;
    int ReturnCode;
    int UserId;

    std::vector<DepotEntry> Depot;
};

} // namespace PassDepot