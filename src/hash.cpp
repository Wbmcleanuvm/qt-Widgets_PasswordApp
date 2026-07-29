//
// Created by wbmdo on 6/19/2026.
//
#include "hash.h"
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>   // for hashing/encryption
#include <openssl/rand.h>  // for random numbers


namespace Hash {
    uint32_t rightrotate(uint32_t x, int n)
    {
        return (x >> n) | (x << (32 - n));
    }
    std::string sha_2(std::string input)
    {
        std::string digest = "";
        //init hash values
        std::vector<uint32_t> hashVals =
        {
            0x6a09e667, // a -> h0
            0xbb67ae85, // b -> h1
            0x3c6ef372, // c -> h2
            0xa54ff53a, // d -> h3
            0x510e527f, // e -> h4
            0x9b05688c, // f -> h5
            0x1f83d9ab, // g -> h6
            0x5be0cd19  // h -> h7
        };
        //copy of hashVals vec before modification
        std::vector<uint32_t> hashVals_h = hashVals;
        std::vector<uint32_t> const roundConsts
        {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
        //padding
        uint64_t L_bits = input.size() * 8;  // L in bits
        std::vector<uint8_t> paddedMsg(input.begin(), input.end());
        // Append the single '1' bit as 0x80 byte
        paddedMsg.push_back(0x80);

        // Append K zero bytes until length = 56 (mod 64)
        // (leaving 8 bytes at the end for the 64-bit length)
        while (paddedMsg.size() % 64 != 56) {
            paddedMsg.push_back(0x00);
        }

        // Append L as a 64-bit big-endian integer
        for (int i = 7; i >= 0; i--) {
            paddedMsg.push_back((L_bits >> (i * 8)) & 0xFF);
        }

        // msg.size() is now a multiple of 64 bytes (512 bits)

        //split into 64 byte chunks

        std::vector<std::vector<uint8_t>> chunks;
        // grab 64 bytes (512 bits) at a time
        for (size_t i = 0; i < paddedMsg.size(); i += 64) {
            // pushes back vec of next 64 bytes
            std::vector<uint8_t> chunk(paddedMsg.begin() + i, paddedMsg.begin() + i + 64);
            chunks.push_back(chunk);
        }

        //chunks into words segment
        std::vector<std::array<uint32_t, 64>> allWords;
        std::array<uint32_t, 64> w;
        uint32_t s0 = 0;
        uint32_t s1 = 0;
        uint32_t ch, temp1,temp2, maj;
        //goes through all chunks of the word
        for (int n = 0; n < chunks.size(); n++){
            std::vector<uint8_t>& word = chunks[n];
            //fill first 16 words
            for(int i = 0; i < 16; i++){
                // combine 4 bytes into one big-endian 32-bit word
                w[i] = (uint32_t)word[i*4]     << 24 |
                    (uint32_t)word[i*4 + 1] << 16 |
                    (uint32_t)word[i*4 + 2] << 8  |
                    (uint32_t)word[i*4 + 3];
            }

            //extend 16 words into the rest of 64 word array
            for(int i = 16; i < 64; i++){
                s0 = rightrotate(w[i-15], 7) ^ rightrotate(w[i-15], 18) ^ (w[i-15] >> 3);
                s1 = rightrotate(w[i-2], 17) ^ rightrotate(w[i-2], 19) ^ (w[i-2] >> 10);
                w[i] = (w[i-16] + s0 + w[i-7] + s1);
            }

            for(int i = 0; i < 64; i++){
                s1 = rightrotate(hashVals[4], 6) ^ rightrotate(hashVals[4], 11) ^ rightrotate(hashVals[4], 25);
                ch = (hashVals[4] & hashVals[5]) ^ ((~hashVals[4]) & hashVals[6]);
                temp1 = hashVals[7] + s1 + ch + w[i] + roundConsts[i];
                s0 = rightrotate(hashVals[0],2) ^ rightrotate(hashVals[0], 13) ^ rightrotate(hashVals[0], 22);
                maj = (hashVals[0] & hashVals[1]) ^ (hashVals[0] & hashVals[2]) ^ (hashVals[1] & hashVals[2]);
                temp2 = s0 + maj;

                hashVals[7] = hashVals[6]; // h = g
                hashVals[6] = hashVals[5]; // g = f
                hashVals[5] = hashVals[4]; // f = e
                hashVals[4] = hashVals[3] + temp1; // e = d + temp1
                hashVals[3] = hashVals[2]; // d = c
                hashVals[2] = hashVals[1]; // c = b
                hashVals[1] = hashVals[0];  //b = a
                hashVals[0] = temp1 + temp2;

            }
            //update hashVals_h with new values
            for(int k = 0; k < hashVals.size(); k++){
                hashVals_h[k] += hashVals[k];
            }
            hashVals = hashVals_h;

        }
        //final string hash to be returned
        for(int i = 0; i < 8; i++){
            char buf[9];
            snprintf(buf, sizeof(buf), "%08x", hashVals_h[i]);
            digest += buf;
        }
        return digest;

    }
    std::string generateSalt(int length)
    {
        unsigned char buf[length];

        // Convert to hex string
        std::string salt;
        for (int i = 0; i < length; i++) {
            char hex[3];
            sprintf(hex, "%02x", buf[i]);
            salt += hex;
        }
        return salt;
    }
    //
    std::string pbkdf2_sha2(std::string passcode,std::string salt, int iterations)
    {
        std::string hash = sha_2(passcode + salt);
        for (int i = 1; i < iterations; i++)
        {
            hash = sha_2(hash);
        }
        return hash;
    }
}



