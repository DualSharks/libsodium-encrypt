// FileEncryptor.cpp
#include "../include/FileEncryptor.h"
#include <fstream>
#include <stdexcept>

FileEncryptor::FileEncryptor(const std::string& fileName, const std::string& password)
        : fileName(fileName) {
    initializeSodium();
    deriveKey(password);
    generateNonces();
}

FileEncryptor::~FileEncryptor() {
    secureErase();
}

void FileEncryptor::encryptFile() {
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open input file.");
    }

    std::ofstream outFile(fileName + "_enc", std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Unable to open output file.");
    }

    processFile(inFile, outFile);
    writeEncryptedData(outFile);
}

void FileEncryptor::initializeSodium() {
    if (sodium_init() == -1) {
        throw std::runtime_error("Libsodium failed to initialize.");
    }
}

void FileEncryptor::deriveKey(const std::string& password) {
    if (crypto_pwhash(key, sizeof key, password.c_str(), password.length(), salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_ARGON2ID13) != 0) {
        throw std::runtime_error("Key derivation failed.");
    }
}

void FileEncryptor::generateNonces() {
    randombytes_buf(masterKeyNonce, sizeof masterKeyNonce);
    randombytes_buf(dataNonce, sizeof dataNonce);
}

void FileEncryptor::processFile(std::ifstream& inFile, std::ofstream& outFile) {
    const std::size_t CHUNK_SIZE = 1024;
    unsigned char buff[CHUNK_SIZE];

    while (inFile) {
        inFile.read(reinterpret_cast<char*>(buff), CHUNK_SIZE);
        std::streamsize bytesRead = inFile.gcount();

        if (bytesRead > 0) {
            crypto_stream_xchacha20_xor(buff, buff, bytesRead, dataNonce, masterKey);
            outFile.write(reinterpret_cast<char*>(buff), bytesRead);
        }
    }
}

void FileEncryptor::writeEncryptedData(std::ofstream& outFile) {
    crypto_stream_xchacha20_xor(masterKey, masterKey, sizeof masterKey, masterKeyNonce, key);
    outFile.write(reinterpret_cast<char*>(&dataNonce), sizeof dataNonce);
    outFile.write(reinterpret_cast<char*>(&masterKeyNonce), sizeof masterKeyNonce);
    outFile.write(reinterpret_cast<char*>(&salt), sizeof salt);
    outFile.write(reinterpret_cast<char*>(&masterKey), sizeof masterKey);
}

void FileEncryptor::secureErase() {
    sodium_memzero(key, sizeof key);
    sodium_memzero(masterKeyNonce, sizeof masterKeyNonce);
    sodium_memzero(dataNonce, sizeof dataNonce);
    sodium_memzero(salt, sizeof salt);
    sodium_memzero(masterKey, sizeof masterKey);
}
