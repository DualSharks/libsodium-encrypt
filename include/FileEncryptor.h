// FileEncryptor.h
#ifndef FILEENCRYPTOR_H
#define FILEENCRYPTOR_H

#include <string>
#include <sodium.h>

class FileEncryptor {
public:
    FileEncryptor(const std::string& fileName, const std::string& password);
    ~FileEncryptor();
    void encryptFile();

private:
    std::string fileName;
    unsigned char key[crypto_stream_xchacha20_KEYBYTES];
    unsigned char salt[crypto_pwhash_argon2id_SALTBYTES];
    unsigned char masterKey[crypto_stream_xchacha20_KEYBYTES];
    unsigned char masterKeyNonce[crypto_stream_xchacha20_NONCEBYTES];
    unsigned char dataNonce[crypto_stream_xchacha20_NONCEBYTES];

    void initializeSodium();
    void deriveKey(const std::string& password);
    void generateNonces();
    void processFile(std::ifstream& inFile, std::ofstream& outFile);
    void writeEncryptedData(std::ofstream& outFile);
    void secureErase();
};

#endif // FILEENCRYPTOR_H
