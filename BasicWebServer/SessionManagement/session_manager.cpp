/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */

#include "session_manager.h"

std::string SessionManager::CreateSession(const std::string& userId)
{
    std::string sessionId = generateSessionId();
    auto expiration = std::chrono::system_clock::now() + std::chrono::seconds(m_SessionTimeout);
    m_SessionsInfo[sessionId] = { userId, expiration };
    return sessionId;
}

bool SessionManager::isSessionAvailable(const std::string& sessionId)
{
    auto it = m_SessionsInfo.find(sessionId);
    if (it != m_SessionsInfo.end() && it->second.expirationTime > std::chrono::system_clock::now()) {
        // Update expiration time on each valid request
        it->second.expirationTime = std::chrono::system_clock::now() + std::chrono::seconds(m_SessionTimeout);
        return true;
    }
    return false;
}

std::string SessionManager::generateSessionId()
{
    // Generate a unique session ID
#if 0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::string sessionId;
    sessionId.reserve(64);

    for (int i = 0; i < 64; ++i) {
        sessionId.push_back(static_cast<char>(dis(gen)));
    }
    return sessionId;
#endif

#if 1
    std::stringstream ss;
    ss << std::hex << std::time(nullptr) << "test"; // Combine timestamp and IP address
    // You may include more factors or use a cryptographic hash function here
    return ss.str();
#endif
}
