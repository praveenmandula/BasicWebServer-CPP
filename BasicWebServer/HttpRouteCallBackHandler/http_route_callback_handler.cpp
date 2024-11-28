/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#include "http_route_callback_handler.h"

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleWelcomePage(HttpStreamHandler::HttpRequest request)
{
    HttpStreamHandler::HttpResponse response;
    std::string htmlStringData;

    LogMessage(LogLevel::INFO, __func__);

    std::string sessionIDCookie;

    // String to search for
    std::string searchString = "Cookie";

    // Find the pair with the given value in the vector
    auto it = std::find_if(request.headers.begin(), request.headers.end(), [&](const auto& pair) {
        return pair.first == searchString;
    });

    if (it != request.headers.end()) {
        LogMessage(LogLevel::INFO, "Session Found");
        sessionIDCookie = it->second;
    }
    else {
        LogMessage(LogLevel::INFO, "Session Not Found");
    }

    std::string sessionID = m_httpStreamHandler.extractSessionIDFromCookie(sessionIDCookie);
    bool sessionAvailable = false;

    if (!sessionID.empty()) {
        sessionID.pop_back();
        sessionAvailable = m_sessionMgr.isSessionAvailable(sessionID);
    }

    int ret;

    // if sessionAvailable No Need to send login page, directly send WelcomePage
    if (sessionAvailable) {
        ret = m_htmlReader.readHtmlContentFromFile("WebHostedFiles/FileUpload.htm", htmlStringData);
    }
    else {
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

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleUserRegistration(HttpStreamHandler::HttpRequest request)
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

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleUserLogin(HttpStreamHandler::HttpRequest request)
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
    usersData.userPassword       = m_httpStreamHandler.UrlDecode(userRegData.at("password"));

    // verify in db, with the user information provided
    verifyUser  =  m_sqlLiteDBManager.verifyUserInformation(usersData);

    if (verifyUser) {
        // create sessionID for this user and send the sessionID in response
        std::string sessionID = m_sessionMgr.CreateSession(usersData.userName);
        if (!sessionID.empty()) {
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

HttpStreamHandler::HttpResponse HttpRouteCallBackHandler::handleFileUpload(HttpStreamHandler::HttpRequest request)
{
    HttpStreamHandler::HttpResponse response;
    // Validate user login
    LogMessage(LogLevel::DEBUG, "handleFileUpload got called,Request details :",request.body);

    // Default filename and file type
    std::string filename = "default_filename.pdf";
    std::string filetype = "application/pdf";

    // Function to search headers for a given key
    auto findHeader = [&](const std::string& key) -> std::string {
        for (const auto& header : request.headers) {
            if (header.first == key) {
                return header.second;
            }
        }
        return "";
    };

    // Extract Content-Disposition header
    std::string contentDisposition = findHeader("Content-Disposition");
    if (!contentDisposition.empty()) {
        size_t filenamePos = contentDisposition.find("filename=");
        if (filenamePos != std::string::npos) {
            filename = contentDisposition.substr(filenamePos + 9); // Extract after "filename="

            LogMessage(LogLevel::INFO,"Before Trim=",filename);
            // Remove any surrounding double quotes from filename
            if (filename.front() == '"' && filename.back() == '"') {
                filename = filename.substr(1, filename.size() - 2);
            }
        }
    }

    // Extract Content-Type header
    filetype = findHeader("Content-Type");
    if (filetype.empty()) {
        filetype = "application/octet-stream"; // Default if not provided
    }

    LogMessage(LogLevel::INFO,"FileName = " , filename);
    int ret = saveFile(filename,request.body);
    if( ret == 0 ) {
        response.statusCode       =   HttpStreamHandler::httpStatusCodes::OK;
        response.statusMessage    =   "OK";
        response.body             =   "File successfully saved";
    }
    else {
        response.statusCode = HttpStreamHandler::httpStatusCodes::FAIL;
        response.statusMessage = "File saving failed";
        response.body = " Error is saving file ";
    }
    return response;
}

int HttpRouteCallBackHandler::saveFile(const std::string& filename, const std::string& fileData) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(fileData.c_str(), fileData.size());
        file.close();
        LogMessage(LogLevel::INFO, "File saved as ", filename);
    } else {
        LogMessage(LogLevel::ERROR_R, "Failed to open file for writing, filename = ", filename);
    	return -1;
    }
    return 0;
}

std::string HttpRouteCallBackHandler::getFilenameFromHeaders(const std::string& headers) {
    // Extract filename from headers (for demonstration, we'll assume it's in a header)
    std::string filename = "uploaded_file.bin";  // Default filename
    size_t pos = headers.find("filename=");
    if (pos != std::string::npos) {
        size_t start = pos + 9; // Length of "filename="
        size_t end = headers.find("\r\n", start);
        filename = headers.substr(start, end - start);
    }
    return filename;
}
