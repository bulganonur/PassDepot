#include "PDDatabase.h"

#include <sqlite3.h>

#include <iostream>

namespace PassDepot
{
    
PDDatabase::PDDatabase() : SqliteDatabase(nullptr), ErrorMessage(nullptr), ReturnCode(0), UserId(0)
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
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        return;
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;
    }

    // Create User table
    const char* SqlCreateUserTable = "CREATE TABLE IF NOT EXISTS User("
        "Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
        "Username TEXT NOT NULL UNIQUE, "
        "MasterPassword TEXT NOT NULL, "
        "Salt TEXT NOT NULL);"; // Salt from DeriveKey()

    ReturnCode = sqlite3_exec(SqliteDatabase, SqlCreateUserTable, nullptr, 0, &ErrorMessage);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << ErrorMessage << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        
        sqlite3_free(ErrorMessage);
    }
    else
    {
        std::cout << "User table created successfully" << std::endl;
    }

    // Create Depot table
    const char* SqlCreateDepotTable = "CREATE TABLE IF NOT EXISTS Depot("
        "Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
        "CipherText TEXT NOT NULL, "
        "Nonce TEXT NOT NULL, "
        "EntryInfo TEXT NOT NULL, "
        "UserId INTEGER NOT NULL, "
        "FOREIGN KEY(UserId) REFERENCES User(Id) ON DELETE RESTRICT);";

    ReturnCode = sqlite3_exec(SqliteDatabase, SqlCreateDepotTable, nullptr, 0, &ErrorMessage);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << ErrorMessage << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_free(ErrorMessage);
    }
    else
    {
        std::cout << "Depot table created successfully" << std::endl;
    }
}

void PDDatabase::InsertDataUserTable(const std::string& Username, const std::string& MasterPassword, const std::string& Salt)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlInsert = "INSERT INTO User (Username, MasterPassword, Salt) VALUES (?, ?, ?);";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    
    // Bind the variables to the SQL statement
    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 2, MasterPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 3, (char*)Salt.data(), -1, SQLITE_STATIC);

    // Execute the statement
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record inserted successfully to User" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::InsertDataDepotTable(const std::string& CipherText, const std::string& Nonce, const std::string& EntryInfo)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlInsert = "INSERT INTO Depot (CipherText, Nonce, EntryInfo, UserId) VALUES (?, ?, ?, ?);";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    
    // Bind the variables to the SQL statement
    sqlite3_bind_text(SqlStatement, 1, (char*)CipherText.data(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 2, (char*)Nonce.data(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 3, EntryInfo.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(SqlStatement, 4, UserId);

    // Execute the statement
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record inserted successfully to Depot" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::UpdateDataDepotTable(int EntryId, const std::string& CipherText, const std::string& Nonce, const std::string& EntryInfo)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlInsert = "UPDATE Depot SET CipherText = ?, Nonce = ?, EntryInfo = ? WHERE Id = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlInsert.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    
    // Bind the variables to the SQL statement
    sqlite3_bind_text(SqlStatement, 1, (char*)CipherText.data(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 2, (char*)Nonce.data(), -1, SQLITE_STATIC);
    sqlite3_bind_text(SqlStatement, 3, EntryInfo.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(SqlStatement, 4, EntryId);

    // Execute the statement
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record updated successfully on Depot" << std::endl;
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(SqlStatement);
}

void PDDatabase::DeleteDataDepotTable(int EntryId)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "DELETE FROM Depot WHERE Id = ? AND UserId = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }

    sqlite3_bind_int(SqlStatement, 1, EntryId);
    sqlite3_bind_int(SqlStatement, 2, UserId);

    // Execute
    ReturnCode = sqlite3_step(SqlStatement);
    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Record deleted successfully from Depot" << std::endl;
    }

    sqlite3_finalize(SqlStatement);
}

void PDDatabase::CacheUserId(const std::string &Username)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Id FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        SetUserId(sqlite3_column_int(SqlStatement, 0));
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }

    sqlite3_finalize(SqlStatement);
}

void PDDatabase::LoadDepot()
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT * FROM Depot WHERE UserId = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }

    sqlite3_bind_int(SqlStatement, 1, UserId);

    // Execute and process the results, load Depot to memory
    Depot.clear();
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        DepotEntry Entry
        (
            sqlite3_column_int(SqlStatement, 0), // EntryId
            reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 1)), // CipherText
            // FromHex(Nonce), // Ignore Nonce
            reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 3)), // EntryInfo 
            sqlite3_column_int(SqlStatement, 4) // UserId
        );
        
        Depot.push_back(Entry);
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return;
    }
    else
    {
        std::cout << "Depot loaded successfully" << std::endl;
    }

    sqlite3_finalize(SqlStatement);
}

bool PDDatabase::DoesUsernameExists(const std::string& Username)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Username FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results, return true if Username already exists
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        sqlite3_finalize(SqlStatement);
        return true;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_finalize(SqlStatement);
    return false;
}

bool PDDatabase::DoesEntryExists(int EntryId)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Id FROM Depot WHERE Id = ? AND UserId = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_bind_int(SqlStatement, 1, EntryId);
    sqlite3_bind_int(SqlStatement, 2, UserId);

    // Execute and process the results, return true if row already exists
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        sqlite3_finalize(SqlStatement);
        return true;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return false;
    }

    sqlite3_finalize(SqlStatement);
    return false;
}

const std::string PDDatabase::GetHash(const std::string &Username)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT MasterPassword FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return nullptr;
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results, if a row exists return its hash
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        std::string Hash = reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 0));

        sqlite3_finalize(SqlStatement);
        return Hash;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return nullptr;
    }

    sqlite3_finalize(SqlStatement);
    return nullptr;
}

const std::string PDDatabase::GetSalt(const std::string &Username)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Salt FROM User WHERE Username = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return std::string();
    }

    sqlite3_bind_text(SqlStatement, 1, Username.c_str(), -1, SQLITE_STATIC);

    // Execute and process the results, if a row exists return its hash
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        const std::string Salt = reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 0));

        sqlite3_finalize(SqlStatement);
        return Salt;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return std::string();
    }

    sqlite3_finalize(SqlStatement);
    return std::string();
}

const std::string PDDatabase::GetNonce(int EntryId)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT Nonce FROM Depot WHERE Id = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return std::string();
    }

    sqlite3_bind_int(SqlStatement, 1, EntryId);

    // Execute and process the results, if a row exists return its hash
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        const std::string Nonce = reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 0));

        sqlite3_finalize(SqlStatement);
        return Nonce;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return std::string();
    }

    sqlite3_finalize(SqlStatement);
    return std::string();
}

DepotEntry PDDatabase::GetEntry(int EntryId)
{
    sqlite3_stmt* SqlStatement;
    const std::string SqlSelect = "SELECT * FROM Depot WHERE Id = ? AND UserId = ?;";

    // Prepare the SQL statement
    ReturnCode = sqlite3_prepare_v2(SqliteDatabase, SqlSelect.c_str(), -1, &SqlStatement, nullptr);
    if (ReturnCode != SQLITE_OK)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return DepotEntry();
    }

    sqlite3_bind_int(SqlStatement, 1, EntryId);
    sqlite3_bind_int(SqlStatement, 2, UserId);

    // Execute and process the results, return the Entry if found
    while ((ReturnCode = sqlite3_step(SqlStatement)) == SQLITE_ROW)
    {
        DepotEntry Entry
        (
            sqlite3_column_int(SqlStatement, 0), // EntryId
            reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 1)), // CipherText
            // FromHex(Nonce), // Ignore Nonce
            reinterpret_cast<const char*>(sqlite3_column_text(SqlStatement, 3)), // EntryInfo 
            sqlite3_column_int(SqlStatement, 4) // UserId
        );
        sqlite3_finalize(SqlStatement);
        return Entry;
    }

    if (ReturnCode != SQLITE_DONE)
    {
        std::cerr << "Err: " << sqlite3_errmsg(SqliteDatabase) << " at: " << __FUNCTION__ << " line:" << __LINE__ << std::endl;
        sqlite3_finalize(SqlStatement);
        return DepotEntry();
    }

    sqlite3_finalize(SqlStatement);
    return DepotEntry();
}

} // namespace PassDepot