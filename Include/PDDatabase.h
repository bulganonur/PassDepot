#pragma once

#include <string>

struct sqlite3;

namespace PassDepot
{

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

    void InsertData(const std::string& Username, const std::string& MasterPassword);
    void ShowDepot();

private:

    void InitDatabaseWithTables();
    
    sqlite3* SqliteDatabase;
    char* ErrorMessage = nullptr;
    int ReturnCode;
};


} // namespace PassDepot
