#pragma once
// Declare static library header references
#include "HttpStreamHandler/http_stream_handler.h"
#include "HtmlReader/html_reader.h"
#include "DBManager/sqlLite_db_manager.h"
#include "LogTrace/log_trace.h"
#include "BasicWebServer/helper_methods.h"

//Web Server related includes
#include "BasicWebServer/SessionManagement/session_manager.h"

class HttpRouteCallBackHandler {
public:
	// Define and implement callback method for requested http routes
	HttpStreamHandler::HttpResponse handleWelcomePage(HttpStreamHandler::HttpRequest& request);
	HttpStreamHandler::HttpResponse handleUserRegistration(HttpStreamHandler::HttpRequest& request);
	HttpStreamHandler::HttpResponse handleUserLogin(HttpStreamHandler::HttpRequest& request);
	HttpStreamHandler::HttpResponse handleFileUpload(HttpStreamHandler::HttpRequest& request);

private:
	bool checkIfSessionExists(HttpStreamHandler::HttpRequest& request);

	HtmlReader           m_htmlReader;
	HttpStreamHandler    m_httpStreamHandler;
	SQLLiteDBManager     m_sqlLiteDBManager;
	SessionManager       m_sessionMgr;
};
