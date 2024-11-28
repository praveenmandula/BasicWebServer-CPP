/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#pragma once
#include "cpp_standard_includes.h"
#include "LogTrace/log_trace.h"

 /**
 HttpStream Handler is used to simplify httpRequest and httpResponse to the users
 takes raw string from the client and converts to user defined httpRequest and viceversa
 **/
class HttpStreamHandler {
public:
    enum httpStatusCodes {
        OK = 200,
        FAIL = 400,
        NOT_FOUND = 500
    };

    struct HttpRequest {
        std::string method;
        std::string path;
        std::string version;
        std::vector<std::pair<std::string, std::string>> headers;
        std::string body;
    };

    struct HttpResponse {
        int statusCode = httpStatusCodes::OK;
        std::string statusMessage;
        std::unordered_map<std::string, std::string> headers;

        std::string body;
        HttpResponse() {
            statusCode = 0;
            headers.insert({ "Content-Type", "text/plain" });
            headers.insert({ "Server", "MyServer/1.0" });
            headers.insert({ "Access-Control-Allow-Origin", "*" });
            headers.insert({ "Access-Control-Allow-Methods", "GET,POST,OPTIONS" });
            headers.insert({ "Access-Control-Allow-Headers", "Origin, Content-Type, Accept" });
        }
    };

    HttpRequest parseHttpRequest(const std::string& requestString);
    std::string generateHTTPResponseStream(HttpStreamHandler::HttpResponse& response);

    typedef std::unordered_map<std::string, std::string> userRegistrationData;
    userRegistrationData parseUserRegistrationRequest(std::string& body);
    std::string UrlDecode(std::string& value);

    std::string extractSessionIDFromCookie(std::string& headers);

private:
    void testParseHttpRequestMethod();
};