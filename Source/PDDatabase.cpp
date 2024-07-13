#include "PDDatabase.h"

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
        "Notes TEXT, "
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

void PDDatabase::InsertData(const std::string &Username, const std::string &MasterPassword)
{
    sqlite3_stmt* SqlStatement;
    const char* SqlInsert = "INSERT INTO User (Username, MasterPassword) VALUES (?, ?);";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert, -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(SqliteDatabase) << std::endl;
        // @todo(Error Handling): ---
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
    }
    else
    {
        std::cout << "Record inserted successfully" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::ShowDepot()
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

} // namespace PassDepot
