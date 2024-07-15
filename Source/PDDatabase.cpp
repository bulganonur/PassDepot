#include "PDDatabase.h"
#include "PDGUI.h"

#include <sqlite3.h>

#include <iostream>

namespace PassDepot
{
    
PDDatabase::PDDatabase()
{
    InitDatabaseWithTables();
}

PDDatabase::~PDDatabase()
{
    // Close the database
    sqlite3_close(SqliteDatabase);
}

void PDDatabase::InitDatabaseWithTables()
{
    // Open the database
    ReturnCode = sqlite3_open("depot.db", &SqliteDatabase);
    if (ReturnCode)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        return;
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;
    }

    // Create User table
    const char* SqlCreateUserTable = "CREATE TABLE IF NOT EXISTS User("
        "Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "Username TEXT NOT NULL, "
        "MasterPassword TEXT NOT NULL);";

    ReturnCode = sqlite3_exec(SqliteDatabase, SqlCreateUserTable, nullptr, 0, &ErrorMessage);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << ErrorMessage << std::endl;
        sqlite3_free(ErrorMessage);
        // @todo(Error Handling): ---
    }
    else
    {
        std::cout << "User table created successfully" << std::endl;
    }

    // Create Depot table
    const char* SqlCreateDepotTable = "CREATE TABLE IF NOT EXISTS Depot("
        "Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "Username TEXT NOT NULL, "
        "Password TEXT NOT NULL, "
        "Notes TEXT NOT NULL, "
        "UserId INTEGER NOT NULL, "
        "FOREIGN KEY(UserId) REFERENCES User(Id) ON DELETE RESTRICT);";

    ReturnCode = sqlite3_exec(SqliteDatabase, SqlCreateDepotTable, nullptr, 0, &ErrorMessage);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << ErrorMessage << std::endl;
        sqlite3_free(ErrorMessage);
        // @todo(Error Handling): ---
    }
    else
    {
        std::cout << "Depot table created successfully" << std::endl;
    }
}

void PDDatabase::InsertDataUserTable(const std::string &Username, const std::string &MasterPassword)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlInsert = "INSERT INTO User (Username, MasterPassword) VALUES (?, ?);";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return;
    }
    
    // Bind the variables to the SQL statement
    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 2, MasterPassword.c_str(), -1, SQLITE_STATIC);
    
    // Execute the statement
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record inserted successfully" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::InsertDataDepotTable(const std::string &Username, const std::string &Password, const std::string &Notes, const int UserId)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlInsert = "INSERT INTO Depot (Username, Password, Notes, UserId) VALUES (?, ?, ?, ?);";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return;
    }
    
    // Bind the variables to the SQL statement
    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 2, Password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 3, Notes.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(SqlStatement, 4, UserId);

    
    // Execute the statement
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record inserted successfully" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::LoadDepot()
{
    // Read data from the table
    const char* SqlSelect = "SELECT * FROM User;";
    
    // Operation to perform with the data provided from the database query (row by row)
    auto SqlCallback = [](void *NotUsed, int ColumnCount, char **ColumnValue, char **ColumnName) -> int
    {
        for (int i = 0; i < ColumnCount; ++i)
        {
            std::cout << ColumnName[i] << ": " << (ColumnValue[i] ? ColumnValue[i] : "NULL") << std::endl;
        }
        std::cout << std::endl;

        return 0;
    };

    ReturnCode = sqlite3_exec(SqliteDatabase, SqlSelect, SqlCallback, 0, &ErrorMessage);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "SQL error: " << ErrorMessage << std::endl;
        sqlite3_free(ErrorMessage);
        // @todo(Error Handling): ---
    }
    else
    {
        std::cout << "Operation done successfully" << std::endl;
    }
}

bool PDDatabase::IsUsernameUnique(const std::string& Username)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Username FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results, return false if Username already exists
    if ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        sqlite3_finalize(SqlStatement);
        return false;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_finalize(SqlStatement);
    return true;
}

bool PDDatabase::Authenticate(const std::string &Username, const std::string &MasterPassword)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT MasterPassword FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results, return true if autentication is successful
    if ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        const unsigned char* ColumnValue = sqlite3_column_text(SqlStatement, 0);
        if (strcmp(MasterPassword.c_str(), (char*)ColumnValue) == 0)
        {
            sqlite3_finalize(SqlStatement);
            return true;
        }
        else
        {
            sqlite3_finalize(SqlStatement);
            return false;
        }
        
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_finalize(SqlStatement);
    return false;
}

} // namespace PassDepot