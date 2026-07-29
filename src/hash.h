//
// Created by wbmdo on 6/19/2026.
//

#ifndef ENCRYPTION_CLION_ENCRYPT_H
#define ENCRYPTION_CLION_ENCRYPT_H
#include <string>
#include <string>
#include <cstdint>

namespace Hash {

    //Sha2 hashing algorithm, takes a password or input and transforms it into a hash
    std::string sha_2(std::string input);
    //right rotate helper function for bit manipulations
    uint32_t rightrotate(uint32_t x, int n);

    //Helper function to generate salt for pkdf2
    std::string generateSalt(int length);

    // uses pkdf2 on a sha2 to make hash unrecognizable less susceptible to brute force attacks Hashes for specified iterations
    std::string pkdf2_sha2(std::string passcode,std::string salt, int iterations);


};

#endif //ENCRYPTION_CLION_ENCRYPT_H