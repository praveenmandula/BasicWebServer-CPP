/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#pragma once
#include "cpp_standard_includes.h"

class SessionManager {
public:
	struct SessionData {
		std::string userID;
		std::chrono::system_clock::time_point expirationTime;
		// Other session data
	};

	std::string CreateSession(const std::string& userId);
	bool isSessionAvailable(const std::string& sessionId);

	typedef std::unordered_map<std::string, SessionData> SessionInfoMap;

private:
	std::string generateSessionId();
	SessionInfoMap m_SessionsInfo;
	const int m_SessionTimeout = 3600; // Session timeout in seconds
};
