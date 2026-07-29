//
// Created by wbmdo on 6/23/2026.
//

#ifndef ENCRYPTION_CLION_VAULT_H
#define ENCRYPTION_CLION_VAULT_H

#include <string>
#include <QStringList>

class Vault {
private:
    std::string dbPath;
    std::string masterKey;
    std::string masterSalt;

public:
    Vault(const std::string& dbPath);
    Vault(const Vault& other);

    //getters
    std::string getDbPath();

    QStringList getPasswords();

    QStringList getUsernames();

    QStringList getSiteNames();

    // Master password
    bool initializeMaster(const std::string& password);
    bool verifyMaster(const std::string& password);

    // Password management
    void addPassword(const std::string& site, const std::string& username, const std::string& password);

    void deletePassword(const std::string& site, const std::string& username);

    bool hasMasterPassword();

};

#endif //ENCRYPTION_CLION_VAULT_H