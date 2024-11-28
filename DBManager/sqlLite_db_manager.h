#pragma once
#include "cpp_standard_includes.h"
#include "LogTrace/log_trace.h"
#include "sqlite3.h"

class SQLLiteDBManager {
public:
	SQLLiteDBManager();

	struct UsersTableData {
		std::string userName;
		std::string userEmaild;
		std::string userPassword;
	};

	int  insertIntoUsersTable(UsersTableData& dataToInsert);
	bool verifyUserInformation(UsersTableData& dataToVerify);

	~SQLLiteDBManager();

private:
	sqlite3* sqlLiteDBInstance = nullptr;
	int testSqlLiteDBLibrary();
};
