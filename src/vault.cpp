//
// Created by wbmdo on 6/23/2026.
//

#include "vault.h"
#include <sqlite3.h>
#include <openssl/rand.h>
#include "hash.h"
#include <QStringList>
#include "encdec.h"
#include <QString>

//constructors

/*Initizalizes an vault object and creates a location on disk for sqlite database
 *@param dbPath - the filepath/ name of the database
 */
Vault::Vault(const std::string& dbPath) : dbPath(dbPath)
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);
    //Creates db, if it exists already do nothing
    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS master (salt TEXT, hash TEXT);"
        "CREATE TABLE IF NOT EXISTS vault (id INTEGER PRIMARY KEY, site TEXT, username TEXT, iv BLOB, password BLOB);",
        nullptr, nullptr, nullptr);

    sqlite3_close(db);
}
Vault::Vault(const Vault& other) : dbPath(other.dbPath), masterKey(other.masterKey), masterSalt(other.masterSalt) {}

/*
 * Initizalizes the master key for the database and stores the hash and salt into a table
 * @param the password for the database
 */
bool Vault::initializeMaster(const std::string& password)
{
    std::string salt = Hash::generateSalt(16);
    std::string hash = Hash::pbkdf2_sha2(password, salt, 100000);

    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO master (salt, hash) VALUES (?, ?);", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, salt.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return true;
}

bool Vault::verifyMaster(const std::string& password)
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT salt, hash FROM master;", -1, &stmt, nullptr);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string salt = (char*)sqlite3_column_text(stmt, 0);
        std::string storedHash = (char*)sqlite3_column_text(stmt, 1);

        std::string inputHash = Hash::pbkdf2_sha2(password, salt, 100000);

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        if (inputHash == storedHash) {
            masterKey = inputHash;
            masterSalt = salt;
            return true;
        }

    }
    return false;
}

std::string Vault::getDbPath()
{
    return this->dbPath;
}

QStringList Vault::getPasswords()
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT site, username, password, iv FROM vault;", -1, &stmt, nullptr);

    QStringList list;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const void* passBlob = sqlite3_column_blob(stmt, 2);
        const void* ivBlob = sqlite3_column_blob(stmt, 3);
        int passSize = sqlite3_column_bytes(stmt, 2);

        std::vector<unsigned char> encrypted((unsigned char*)passBlob, (unsigned char*)passBlob + passSize);
        std::string password = Encdec::decrypt(encrypted, masterKey, (unsigned char*)ivBlob);

        list << QString::fromStdString(password);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return list;
}

QStringList Vault::getSiteNames()
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT site, username, password, iv FROM vault;", -1, &stmt, nullptr);

    QStringList list;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        QString site = (char*)sqlite3_column_text(stmt, 0);
        list << site;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return list;
}

QStringList Vault::getUsernames()
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT site, username, password, iv FROM vault;", -1, &stmt, nullptr);

    QStringList list;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        QString username = (char*)sqlite3_column_text(stmt, 1);
        list << username;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return list;

}
void Vault::addPassword(const std::string& site, const std::string& username, const std::string& password)
{
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));

    auto encrypted = Encdec::encrypt(password, masterKey, iv);

    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "INSERT INTO vault (site, username, iv, password) VALUES (?, ?, ?, ?);", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, site.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 3, iv, sizeof(iv), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, encrypted.data(), encrypted.size(), SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}


bool Vault::hasMasterPassword()
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM master;", -1, &stmt, nullptr);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return count > 0;
}
void Vault::deletePassword(const std::string& site, const std::string& username)
{
    sqlite3* db;
    sqlite3_open(dbPath.c_str(), &db);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "DELETE FROM vault WHERE site = ? AND username = ?;", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, site.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}