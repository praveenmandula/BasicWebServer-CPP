/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */

#pragma once
// Declare static library header references
#include "HttpStreamHandler/http_stream_handler.h"
#include "HtmlReader/html_reader.h"
#include "DBManager/sqlLite_db_manager.h"
#include "LogTrace/log_trace.h"

//Web Server related includes
#include "BasicWebServer/SessionManagement/session_manager.h"

class HttpRouteCallBackHandler {
public:
	// Declare and implement callback method for requested http routes
	HttpStreamHandler::HttpResponse handleWelcomePage(HttpStreamHandler::HttpRequest request);
	HttpStreamHandler::HttpResponse handleUserRegistration(HttpStreamHandler::HttpRequest request);
	HttpStreamHandler::HttpResponse handleUserLogin(HttpStreamHandler::HttpRequest request);
	HttpStreamHandler::HttpResponse handleFileUpload(HttpStreamHandler::HttpRequest request);

private:
	// private methods
	int saveFile(const std::string& filename, const std::string& fileData);
	std::string getFilenameFromHeaders(const std::string& headers);

	// helper methods
	HtmlReader           m_htmlReader;
	HttpStreamHandler    m_httpStreamHandler;
	SQLLiteDBManager     m_sqlLiteDBManager;

	// Session Manager
	SessionManager    m_sessionMgr;
};
