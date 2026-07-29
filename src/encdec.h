//
// Created by wbmdo on 6/23/2026.
//
#include <string>
#include <vector>
#ifndef ENCRYPTION_CLION_ENCDEC_H
#define ENCRYPTION_CLION_ENCDEC_H
namespace Encdec {
    //Encrypts a file using aes_256
    std::vector<unsigned char> encrypt(const std::string& plaintext, const std::string& key, const unsigned char* iv);

    //decrypt
    std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key, const unsigned char* iv);

}
#endif //ENCRYPTION_CLION_ENCDEC_H