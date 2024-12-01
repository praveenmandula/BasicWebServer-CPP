/*
 * Author: Praveen Mandula
 * Date: 11/04/2024
 * Description : HttpStreamHandler class is used for converting client request raw data into userdefined HttpRequest and viceversa
 */
#include "http_stream_handler.h"

HttpStreamHandler::HttpRequest HttpStreamHandler::parseHttpRequest(const std::string& requestString)
{
    HttpRequest request;

#if 0
    // Find method
    size_t methodEnd = requestString.find(' ');
    request.method = requestString.substr(0, methodEnd);

    // Find path
    size_t pathEnd = requestString.find(' ', methodEnd + 1);
    request.path = requestString.substr(methodEnd + 1, pathEnd - methodEnd - 1);

    // Find version
    size_t versionEnd = requestString.find("\r\n", pathEnd + 1);
    request.version = requestString.substr(pathEnd + 1, versionEnd - pathEnd - 1);

    // Find headers
    size_t headerStart = versionEnd + 2; // Skip "\r\n"
    size_t headerEnd = requestString.find("\r\n\r\n", headerStart);
    std::string headersString = requestString.substr(headerStart, headerEnd - headerStart);
    size_t pos = 0;
    while ((pos = headersString.find("\r\n", pos)) != std::string::npos) {
        request.headers.push_back(headersString.substr(0, pos));
        pos += 2; // Skip "\r\n"
    }

    // Find body
    request.body = requestString.substr(headerEnd + 4); // Skip "\r\n\r\n"
#endif

#if 1
    std::istringstream stream(requestString);
    std::string line;

    // Parse the request line
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> request.method >> request.path >> request.version;

    // Parse headers
    while (std::getline(stream, line) && !line.empty()) {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 2); // Skip ": " after the colon
            request.headers[headerName] = headerValue; // Insert into the map
        }
    }

    // Find the position of the empty line after the headers
    size_t pos = requestString.find("\r\n\r\n");

    // get the body
    if (pos != std::string::npos) {
        // Extract the body from the position after the empty line
        request.body = requestString.substr(pos + 4);
    }
#endif
    return request;
}

std::string HttpStreamHandler::generateHTTPResponseStream(HttpStreamHandler::HttpResponse& httpResponse)
{
    std::ostringstream response;

    // Status line
    response << "HTTP/1.1 " << httpResponse.statusCode << " " << httpResponse.statusMessage << "\r\n";

    // Headers
    for (const auto& header : httpResponse.headers) {
        response << header.first << ": " << header.second << "\r\n";
    }

    // Content length
    response << "Content-Length: " << httpResponse.body.size() << "\r\n";

    // Empty line before the body
    response << "\r\n";

    // Body
    response << httpResponse.body;

    return response.str();
}

HttpStreamHandler::userRegistrationData HttpStreamHandler::parseUserRegistrationRequest(std::string& body)
{
    std::unordered_map<std::string, std::string> params;
    size_t pos = 0;
    while (pos < body.length()) {
        size_t equalPos = body.find('=', pos);
        size_t ampersandPos = body.find('&', pos);
        size_t keyLen = (equalPos != std::string::npos) ? equalPos - pos : ampersandPos - pos;
        std::string key = body.substr(pos, keyLen);

        if (equalPos != std::string::npos) {
            size_t valueLen = (ampersandPos != std::string::npos) ? ampersandPos - equalPos - 1 : std::string::npos;
            std::string value = body.substr(equalPos + 1, valueLen);
            params[key] = value;
        }
        else {
            // If no equal sign found, assume empty value
            params[key] = "";
        }

        if (ampersandPos == std::string::npos) {
            break;
        }
        pos = ampersandPos + 1;
    }
    return params;
}

std::string HttpStreamHandler::UrlDecode(std::string& value)
{
    std::ostringstream unescaped;
    for (size_t i = 0; i < value.length(); ++i) {
        if (value[i] == '%' && i + 2 < value.length()) {
            int hexValue;
            std::istringstream hexStream(value.substr(i + 1, 2));
            hexStream >> std::hex >> hexValue;
            unescaped << static_cast<char>(hexValue);
            i += 2;
        }
        else if (value[i] == '+') {
            unescaped << ' ';
        }
        else {
            unescaped << value[i];
        }
    }
    return unescaped.str();
}

void HttpStreamHandler::testParseHttpRequestMethod()
{
#if 0
    std::string httpRequestString =
        "GET /index.html HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "Connection: keep-alive\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "\r\n"
        "This is the body of the request.";

    HttpRequest httpRequest = parseHttpRequest(httpRequestString);

    // Print parse httpRequest information
    LogMessage(LogLevel::DEBUG, "Method = ", httpRequest.method);
    LogMessage(LogLevel::DEBUG, "Path = ", httpRequest.path);
    LogMessage(LogLevel::DEBUG, "Version = ", httpRequest.version);
    LogMessage(LogLevel::DEBUG, "Headers = ");
    for (const auto& header : httpRequest.headers) {
        LogMessage(LogLevel::DEBUG, header.first , "  " , header.second);
    }
    LogMessage(LogLevel::DEBUG, "Body = ", httpRequest.body);
#endif
}

std::string HttpStreamHandler::extractSessionIDFromCookie(std::string& headers)
{
    std::string sessionID;
    size_t pos = headers.find("session_id=: ");
    if (pos != std::string::npos) {
        sessionID = headers.substr(pos + std::string("session_id=: ").length());
    }
    return sessionID;
}
