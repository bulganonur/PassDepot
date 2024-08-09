#pragma once

#include <string>
#include <vector>

struct sqlite3;

namespace PassDepot
{

/**
 * Used as a buffer while loading the Depot to memory
 */
struct DepotEntry
{
    DepotEntry(int InEntryId, const std::string& InCipherText, const std::string& InEntryInfo, int InUserId)
    : EntryId(InEntryId), CipherText(InCipherText), EntryInfo(InEntryInfo), UserId(InUserId)
    {}

    DepotEntry(){};

    int EntryId;
    std::string CipherText;
    std::string EntryInfo;
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

    void InsertDataUserTable(const std::string& Username, const std::string& MasterPassword, const std::string& Salt);

    void InsertDataDepotTable(const std::string& CipherText, const std::string& Nonce, const std::string& EntryInfo);

    void UpdateDataDepotTable(int EntryId, const std::string& CipherText, const std::string& Nonce, const std::string& EntryInfo);

    void DeleteDataDepotTable(int EntryId);

    // Gets UserId for the currently logged in user and caches it, so that data related to only that user is processed 
    void CacheUserId(const std::string& Username);

    // Load Depot table into memory
    void LoadDepot();

    // Query database for the Username given
    bool DoesUsernameExists(const std::string& Username);

    // Query database for the Id given
    bool DoesEntryExists(int EntryId);

    // Returns hashed password of the User
    const std::string GetHash(const std::string& Username);

    // Returns salt used for deriving the key of the User
    const std::string GetSalt(const std::string& Username);

    // Return nonce used to encrypt this entry
    const std::string GetNonce(int EntryId);

    // Get a specific row from database
    DepotEntry GetEntry(int EntryId);

    // Getter for the Depot
    inline const std::vector<DepotEntry>& GetDepot() const { return Depot; }

    // Setter for the UserId
    inline void SetUserId(int InUserId) { UserId = InUserId; }
    
private:

    void InitDatabaseWithTables();
    
    sqlite3* SqliteDatabase;
    char* ErrorMessage;
    int ReturnCode;
    int UserId;

    std::vector<DepotEntry> Depot;
};

} // namespace PassDepot