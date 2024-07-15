#pragma once

#include <string>
#include <vector>

struct sqlite3;

namespace PassDepot
{

struct DepotEntry
{
    int EntryId;
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
    void InsertDataDepotTable(const std::string& Username, const std::string& Password, const std::string& Notes, const int UserId);
    void LoadDepot();
    bool IsUsernameUnique(const std::string& Username);
    bool Authenticate(const std::string& Username, const std::string& MasterPassword);
    // @todo: save session/user id after login and register

private:

    void InitDatabaseWithTables();
    
    sqlite3* SqliteDatabase;
    char* ErrorMessage = nullptr;
    int ReturnCode;

    std::vector<DepotEntry> Depot;
};

} // namespace PassDepot