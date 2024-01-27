#include "include/FileEncryptor.h"
#include <iostream>
#include <string>


int main() {
    try {
        std::string fileName;
        std::cout << "Enter the name of the file to encrypt: ";
        getline(std::cin, fileName);

        std::string password;
        std::cout << "Enter the password (max 32 chars): ";
        getline(std::cin, password);

        FileEncryptor encryptor(fileName, password);
        encryptor.encryptFile();

        std::cout << "File encrypted successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
