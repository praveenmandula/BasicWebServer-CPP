/*
 * Author: Praveen Mandula
 * Date: 11/22/2024
 * Description : HttpRouteCallBackHandler class methods serves client request for a specific http route and sends backs http response
 */
#include "http_route_callback_handler.h"

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleWelcomePage(HttpStreamHandler::HttpRequest& request)
{
    HttpStreamHandler::HttpResponse response;
    std::string htmlStringData;

    LogMessage(LogLevel::INFO, __func__);

    bool sessionAvailable = checkIfSessionExists(request);

    int ret;

    // if sessionAvailable No Need to send login page, directly send WelcomePage
    if (sessionAvailable) {
        LogMessage(LogLevel::INFO, "Session available,Skipping Login");
        ret = m_htmlReader.readHtmlContentFromFile("WebHostedFiles/FileUpload.htm", htmlStringData);
    }
    else {
        LogMessage(LogLevel::INFO, "Session not available,Please Login");
        ret = m_htmlReader.readHtmlContentFromFile("WebHostedFiles/WelcomePageAjax.htm", htmlStringData);
    }
    
    if (ret == 0) {
        response.statusCode = HttpStreamHandler::httpStatusCodes::OK;
        response.statusMessage = "OK";
        
        // update headers
        auto it = response.headers.find("Content-Type");
        if (it != response.headers.end()) {
            it->second = "text/html";
        }
        response.body = htmlStringData;
    }
    else {
        LogMessage(LogLevel::ERROR_R,"Page Not Found");
        std::string body;
        body.append("page not found");
        body.append("\n");
        response.statusCode = HttpStreamHandler::httpStatusCodes::NOT_FOUND;
        response.statusMessage = "ERROR";
        response.body = body;
    }
    return response;
}

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleUserRegistration(HttpStreamHandler::HttpRequest& request)
{
    HttpStreamHandler::HttpResponse response;

    LogMessage(LogLevel::INFO, __func__);
    LogMessage(LogLevel::DEBUG,"Request body = ",request.body);

    HttpStreamHandler::userRegistrationData userRegData = m_httpStreamHandler.parseUserRegistrationRequest(request.body);

    // Read user registration from the parsed userRegData and assign it to the UsersTableData
    SQLLiteDBManager::UsersTableData usersData;
    usersData.userName      =  m_httpStreamHandler.UrlDecode(userRegData.at("username"));
    usersData.userEmaild    =  m_httpStreamHandler.UrlDecode(userRegData.at("email"));
    usersData.userPassword  =  m_httpStreamHandler.UrlDecode(userRegData.at("password"));

    int ret = m_sqlLiteDBManager.insertIntoUsersTable(usersData);

    if (ret == 0) {
        response.statusCode = HttpStreamHandler::httpStatusCodes::OK;
        response.statusMessage = "OK";
        response.body = "Registration successful";
    }
    else {
        response.statusCode = HttpStreamHandler::httpStatusCodes::FAIL;
        response.statusMessage = "Failed";
        response.body = "Registration Failed";
    } 
    return response;
}

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleUserLogin(HttpStreamHandler::HttpRequest& request)
{
    HttpStreamHandler::HttpResponse response; 

    LogMessage(LogLevel::INFO, __func__);
    LogMessage(LogLevel::DEBUG, request.body);

    if (request.body.empty())
        return response;

    bool verifyUser = false;
    // parse request body into userdefined structure
    HttpStreamHandler::userRegistrationData userRegData = m_httpStreamHandler.parseUserRegistrationRequest(request.body);

    // Read user registration from the parsed userRegData and assign it to the UsersTableData
    SQLLiteDBManager::UsersTableData usersData;
    usersData.userName           =  m_httpStreamHandler.UrlDecode(userRegData.at("username"));
    usersData.userPassword       =  m_httpStreamHandler.UrlDecode(userRegData.at("password"));

    // verify in db, with the user information provided
    verifyUser  =  m_sqlLiteDBManager.verifyUserInformation(usersData);

    if (verifyUser) {
        LogMessage(LogLevel::INFO, "Login Success");
        // create sessionID for this user and send the sessionID in response
        std::string sessionID = m_sessionMgr.CreateSession(usersData.userName);
        if (!sessionID.empty()) {
            LogMessage(LogLevel::INFO, "Session Created for the User - ", usersData.userName);
            response.headers.insert(std::make_pair("Set-Cookie: session_id=", sessionID));
        }

        std::string htmlStringData;
        int ret = m_htmlReader.readHtmlContentFromFile("WebHostedFiles/FileUpload.htm", htmlStringData);

        if (ret == 0) {
            // save login info in the session
            response.statusCode = HttpStreamHandler::httpStatusCodes::OK;
            response.statusMessage = "OK";
            
            // update headers
            auto it = response.headers.find("Content-Type");
            if (it != response.headers.end()) {
                it->second = "text/html";
            }
            response.body = htmlStringData;
        }
        else {
        	LogMessage(LogLevel::ERROR_R, "login success page not found");
            std::string body;
            body.append("login success page not found");
            body.append("\n");
            response.statusCode = HttpStreamHandler::httpStatusCodes::NOT_FOUND;
            response.statusMessage = "Invalid Credentials";
            response.body = body;
        }
    }
    else {
        LogMessage(LogLevel::ERROR_R, "Invalid Credentials");
        std::string body;
        body.append("Invalid Credentials");
        body.append("\n");
        response.statusCode = HttpStreamHandler::httpStatusCodes::NOT_FOUND;
        response.statusMessage = "ERROR";
        response.body = body;
    }
    return response;
}

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleFileUpload(HttpStreamHandler::HttpRequest& request)
{
    HttpStreamHandler::HttpResponse response;
    // Validate user login
    LogMessage(LogLevel::INFO, __func__);
    LogMessage(LogLevel::DEBUG, "handleFileUpload got called,Request details :",request.body);

    bool sessionAvailable = checkIfSessionExists(request);

    if (!sessionAvailable) {
        return handleWelcomePage(request);
    }

    LogMessage(LogLevel::INFO, "Received body length = ", request.body.length());
        
    // Default filenames and file type
    std::string filename = "uploaded_";
    std::string filetype = "application/octet-stream";
    std::string fileData;

    // Regular expression to extract filename
    std::regex filenameRegex(R"(filename=\"([^\"]+)\")");
    std::smatch filenameMatch;
    if (std::regex_search(request.body, filenameMatch, filenameRegex)) {
        filename = filenameMatch[1];  // Extract filename
    }

    // Regular expression to extract content type (filetype)
    std::regex contentTypeRegex(R"(Content-Type:\s*([^\s]+))");
    std::smatch contentTypeMatch;
    if (std::regex_search(request.body, contentTypeMatch, contentTypeRegex)) {
        filetype = contentTypeMatch[1];  // Extract content type
    }

    // Regex pattern to match the content between two \n\n (empty lines)
    std::regex fileDataPattern(R"(\r?\n\r?\n([\s\S]*?)\r?\n\r?\n)");
    // Perform the regex match
    std::smatch fileDatamatch;
    if (std::regex_search(request.body, fileDatamatch, fileDataPattern)) {
        // The first captured group (index 1) contains the file data (body)
        fileData = fileDatamatch[1].str();
    }

    LogMessage(LogLevel::INFO, "FileName = " , filename);
    LogMessage(LogLevel::INFO, "FileType = ", filetype);
    LogMessage(LogLevel::INFO, "FileDataRecvd, length = ", fileData.length());

    int ret = HelperMethods::saveFile(filename, filetype, fileData);
    if (ret == 0) {
        response.statusCode = HttpStreamHandler::httpStatusCodes::OK;
        response.statusMessage = "File successfully saved";
    }
    else {
        response.statusCode = HttpStreamHandler::httpStatusCodes::FAIL;
        response.statusMessage = "File saving failed";
    }
    return response;
}

bool HttpRouteCallBackHandler::checkIfSessionExists(HttpStreamHandler::HttpRequest& request)
{
    std::string sessionIDCookie;

    // String to search for
    std::string searchString = "Cookie";

    // Find the pair with the given value in the map
    auto it = std::find_if(request.headers.begin(), request.headers.end(), [&](const auto& pair) {
        return pair.first == searchString;
    });

    if (it != request.headers.end()) {
        sessionIDCookie = it->second;
    }

    std::string sessionID = m_httpStreamHandler.extractSessionIDFromCookie(sessionIDCookie);

    bool sessionAvailable = false;
    if (!sessionID.empty()) {
        sessionID.pop_back();
        sessionAvailable = m_sessionMgr.isSessionAvailable(sessionID);
    }
    return sessionAvailable;
}
