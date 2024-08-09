#pragma once

#include <sodium.h>

#include <string>
#include <vector>

namespace PassDepot
{

/**
 * Wrapper class for libsodium operations
 */
class PDSecurity
{
  
public:

    PDSecurity();
    ~PDSecurity();

    // delete copy ctor and copy assignment operator to prevent copies
    PDSecurity(const PDSecurity&) = delete;
    PDSecurity& operator=(const PDSecurity&) = delete;

public:

    void Init();

    // Hash a given Password. Salted by default with sodium's pwhash_str api. Params (memory and operation costs/limits, 
    // salt and algorithm) used for hashing are concatenated to the hash itself. Thus verification (see VerifyPassword) 
    // only requires a hash and a password
    std::string HashPassword(const std::string& Password);

    // Password verification. Compares an unhashed password with a hashed one, returns true if they match
    bool VerifyPassword(const std::string& Hash, const std::string& Password);

    // Encrypts PlainText with the InKey into OutCipherText. A random nonce is generated internally. 
    // returns the nonce if encryption is successful
    const std::vector<unsigned char> Encrypt(std::vector<unsigned char>& OutCipherText, const std::string& PlainText, const std::vector<unsigned char>& InKey);
    
    // Decrypts CipherText(hexadecimal string) into OutPlainText, using InNonce and InKey
    void Decrypt(std::vector<unsigned char>& OutPlainText, const std::string& CipherText, const std::string& InNonce, const std::vector<unsigned char>& InKey);
    
    // Generates a random salt that can be used for deriving a key
    const std::string GenerateSalt();

    // Derives a key from Password and InSalt that can be used for encryption/decryption. Key is a member variable and 
    // can be accessed via its getter; GetKey()
    void DeriveKey(const std::string& Password, const std::string& InSalt);

    // Used in between logins
    void ClearKey();

    // Getter for Key
    inline const std::vector<unsigned char>& GetKey() const { return Key; }

    const std::string BinToHex(const std::vector<unsigned char> Bin);
    const std::vector<unsigned char> HexToBin(const std::string& Hex);

    // @todo: Both of these functions are unused atm. Leaving them here for future implementation of VirtualMemoryLock.
    void MemLock(void* MemAddress);
    void MemUnlock(void* MemAddress);

private:

    std::vector<unsigned char> Key;

    /**
     * Some constants from libsodium
     */

    // crypto_pwhash_OPSLIMIT_MODERATE
    // crypto_pwhash_MEMLIMIT_MODERATE
    // crypto_pwhash_ALG_DEFAULT
    // crypto_pwhash_BYTES_MAX
    // crypto_pwhash_BYTES_MIN
    // crypto_pwhash_PASSWD_MAX
    // crypto_pwhash_PASSWD_MIN
    // crypto_pwhash_SALTBYTES
    // crypto_pwhash_STRBYTES
    // crypto_secretbox_MACBYTES
    // crypto_secretbox_KEYBYTES
    // crypto_secretbox_NONCEBYTES
    // crypto_box_SEEDBYTES

};

} // namespace PassDepot
