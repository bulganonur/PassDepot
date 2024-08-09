#include "PDSecurity.h"

#include <iostream>
#include <source_location>

namespace PassDepot
{

PDSecurity::PDSecurity() : Key(crypto_secretbox_KEYBYTES)
{
    Init();
}

PDSecurity::~PDSecurity()
{
}

void PDSecurity::Init()
{
    if (sodium_init() < 0)
    {
        std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
    }
}

std::string PDSecurity::HashPassword(const std::string& Password)
{
    char OutHash[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(OutHash, Password.c_str(), Password.length(), crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE) == 0)
    {
        return OutHash;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
    return std::string();
}

bool PDSecurity::VerifyPassword(const std::string& Hash, const std::string& Password)
{
    return crypto_pwhash_str_verify(Hash.c_str(), Password.c_str(), Password.length()) == 0;
}

const std::vector<unsigned char> PDSecurity::Encrypt(std::vector<unsigned char>& OutCipherBin, const std::string& PlainText, const std::vector<unsigned char>& InKey)
{
    std::vector<unsigned char> Nonce(crypto_secretbox_NONCEBYTES);
    randombytes_buf(Nonce.data(), Nonce.size());

    if (crypto_secretbox_easy(OutCipherBin.data(), reinterpret_cast<const unsigned char*>(PlainText.c_str()), PlainText.length(), Nonce.data(), InKey.data()) == 0)
    {
        return Nonce;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
    return std::vector<unsigned char>();
}

void PDSecurity::Decrypt(std::vector<unsigned char>& OutPlainText, const std::string& CipherText, const std::string& InNonce, const std::vector<unsigned char>& InKey)
{
    const std::vector<unsigned char> CipherBin = HexToBin(CipherText);
    const std::vector<unsigned char> NonceBin = HexToBin(InNonce);

    if (crypto_secretbox_open_easy(OutPlainText.data(), CipherBin.data(), CipherBin.size(), NonceBin.data(), InKey.data()) == 0)
    {
        return;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
}

const std::string PDSecurity::GenerateSalt()
{
    std::vector<unsigned char> Salt(crypto_pwhash_SALTBYTES);
    randombytes_buf(Salt.data(), Salt.size());

    return BinToHex(Salt);
}

void PDSecurity::DeriveKey(const std::string& Password, const std::string& InSalt)
{
    // If key has been cleared out, resize
    if (Key.empty())
    {
        Key.resize(crypto_secretbox_KEYBYTES);
    }

    const std::vector<unsigned char> SaltBin = HexToBin(InSalt);
    
    if (crypto_pwhash(Key.data(), Key.size(), Password.c_str(), Password.length(), SaltBin.data(), crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_DEFAULT) == 0)
    {
        return;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
}

void PDSecurity::ClearKey()
{
    // Overwrite the contents with zeros
    if (!Key.empty())
    {
        sodium_memzero(Key.data(), Key.size());
    }
}

const std::string PDSecurity::BinToHex(const std::vector<unsigned char> Bin)
{
    std::string OutHex;
    OutHex.resize(Bin.size() * 2);
    
    sodium_bin2hex(OutHex.data(), OutHex.length() + 1, Bin.data(), Bin.size());

    return OutHex;
}

const std::vector<unsigned char> PDSecurity::HexToBin(const std::string &Hex)
{
    std::vector<unsigned char> OutBin(Hex.length() / 2);
    size_t OutBinLen; 
    if (sodium_hex2bin(OutBin.data(), OutBin.size(), Hex.c_str(), Hex.length(), nullptr, &OutBinLen, nullptr) == 0)
    {
        OutBin.resize(OutBinLen);
        return OutBin;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
    return std::vector<unsigned char>();
}

void PDSecurity::MemLock(void* MemAddress)
{
    if (sodium_mlock(MemAddress, sizeof(MemAddress)) == 0)
    {
        return;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
}

void PDSecurity::MemUnlock(void* MemAddress)
{
    if (sodium_munlock(MemAddress, sizeof(MemAddress)) == 0)
    {
        return;
    }
    std::cerr << "Execution failed at: " << std::source_location::current().function_name() << std::endl;
}

} // namespace PassDepot
