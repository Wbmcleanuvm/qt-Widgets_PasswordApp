//
// Created by wbmdo on 6/23/2026.
//

#include "encdec.h"
#include <openssl/evp.h>
#include <string>
#include <vector>
namespace Encdec
{
    std::vector<unsigned char> encrypt(const std::string& plaintext, const std::string& key, const unsigned char* iv)
    {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
            (unsigned char*)key.c_str(), iv);

        std::vector<unsigned char> ciphertext(plaintext.size() + 16);
        int len = 0, totalLen = 0;

        EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
            (unsigned char*)plaintext.c_str(), plaintext.size());
        totalLen += len;

        EVP_EncryptFinal_ex(ctx, ciphertext.data() + totalLen, &len);
        totalLen += len;

        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(totalLen);
        return ciphertext;
    }

    std::string decrypt(const std::vector<unsigned char>& ciphertext, const std::string& key, const unsigned char* iv)
    {

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
            (unsigned char*)key.c_str(), iv);

        std::vector<unsigned char> plaintext(ciphertext.size());
        int len = 0, totalLen = 0;

        EVP_DecryptUpdate(ctx, plaintext.data(), &len,
            ciphertext.data(), ciphertext.size());
        totalLen += len;

        EVP_DecryptFinal_ex(ctx, plaintext.data() + totalLen, &len);
        totalLen += len;

        EVP_CIPHER_CTX_free(ctx);
        return std::string(plaintext.begin(), plaintext.begin() + totalLen);
    }
}