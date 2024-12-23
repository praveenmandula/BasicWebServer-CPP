/*
 * Author: Praveen Mandula
 * Date: 10/30/2024
 * Description : SQLLiteDBManager class is used for preparing and executing sqlLite db queries
 */
#include "sqlLite_db_manager.h"

SQLLiteDBManager::SQLLiteDBManager()
{
    char* errMsg = nullptr;
    // Open database (create if not exists)
    int rc = sqlite3_open("test.db", &sqlLiteDBInstance);
    if (rc) {
        LogMessage(LogLevel::ERROR_R, "Unable to Open Database: ", (char*)sqlite3_errmsg(sqlLiteDBInstance));
    }
    else {
        LogMessage(LogLevel::INFO, "Opened Database Successfully ");
    }

    // Create USERS table
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Users (UName TEXT UNIQUE,UEmail TEXT UNIQUE,UPass TEXT);";
    rc = sqlite3_exec(sqlLiteDBInstance, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        LogMessage(LogLevel::ERROR_R, "Can't Access USERS TABLE SQL error:", (char*)errMsg);
        sqlite3_free(errMsg);
    }
    else {
        LogMessage(LogLevel::DEBUG, " Table Created Successfully ");
    }
}

int SQLLiteDBManager::insertIntoUsersTable(UsersTableData& dataToInsert)
{
    int rc;
    // Create SQL Statement
    std::string InsertStatementString = "INSERT INTO Users (UName, UEmail, UPass) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    // Prepare SQL Statement
    rc = sqlite3_prepare_v2(sqlLiteDBInstance, InsertStatementString.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        LogMessage(LogLevel::ERROR_R, "SQL error while inserting UsersData: ",sqlite3_errmsg(sqlLiteDBInstance));
        return -1;
    }

    // Bind Parameters
    sqlite3_bind_text(stmt, 1, dataToInsert.userName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dataToInsert.userEmaild.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, dataToInsert.userPassword.c_str(), -1, SQLITE_STATIC);

    // Execute Statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        LogMessage(LogLevel::ERROR_R, "SQL error while executing statement: ", sqlite3_errmsg(sqlLiteDBInstance));
        return -1;
    }

    // Finalize Statement
    sqlite3_finalize(stmt);

    return 0;
}

bool SQLLiteDBManager::verifyUserInformation(UsersTableData& dataToVerify)
{
    // Prepare SQL Statement
    std::string sql = "SELECT * FROM Users WHERE UName=? AND UPass=?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(sqlLiteDBInstance, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        LogMessage(LogLevel::ERROR_R, "SQL error while preparing statement: ", sqlite3_errmsg(sqlLiteDBInstance));
        sqlite3_close(sqlLiteDBInstance);
        return false;
    }

    // Bind Parameters
    sqlite3_bind_text(stmt, 1, dataToVerify.userName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dataToVerify.userPassword.c_str(), -1, SQLITE_STATIC);

    // Execute Statement
    rc = sqlite3_step(stmt);
    bool verified = (rc == SQLITE_ROW);

    // Finalize Statement
    sqlite3_finalize(stmt);

    return verified;
}

SQLLiteDBManager::~SQLLiteDBManager()
{
    // Close database
    if (sqlLiteDBInstance != nullptr)
        sqlite3_close(sqlLiteDBInstance);
}

// test method
int SQLLiteDBManager::testSqlLiteDBLibrary()
{
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    // Open database
    rc = sqlite3_open("test.db", &db);
    if (rc) {
        LogMessage(LogLevel::ERROR_R, " Unable to OpenDatabase: ", (char*)sqlite3_errmsg(db));
        return 1;
    }

    // Execute SQL
    const char* sql = "CREATE TABLE IF NOT EXISTS MyTable (ID INT, Name TEXT);";
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        LogMessage(LogLevel::ERROR_R, "SQL error while creating Table: ", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        LogMessage(LogLevel::INFO, " Table Created Successfully ");
    }

    // Close database
    sqlite3_close(db);
    return 0;
}
