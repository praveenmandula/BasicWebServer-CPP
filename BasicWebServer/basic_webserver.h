#pragma once
 // common cpp includes
#include "cpp_standard_includes.h"

// static libraries headers
#include "TcpSocketHandler/tcp_socket_handler.h"
#include "HttpStreamHandler/http_stream_handler.h"
#include "HtmlReader/html_reader.h"
#include "DBManager/sqlLite_db_manager.h"

// DLL library includes
#include "LogTrace/log_trace.h"

// HttpRouteCallBack headers
#include "HttpRouteCallBackHandler/http_route_callback_handler.h"

/**
BasicWebServer class is used to create a webserver on a specified port using TcpSocketHandler class
BasicWebServer user need to register callback handlers for the requested routes using HttpRouteCallbackHandlers class methods
BasicWebServer will call appropriate callback handler based on requested route from the client
**/

class BasicWebServer {
public:
	BasicWebServer() = delete;
	BasicWebServer(int port);
	
	int acceptConnections();
	void registerHttpRouteCallbacks();

private:
	// Register callbacks
	typedef std::function<HttpStreamHandler::HttpResponse(HttpStreamHandler::HttpRequest& request)> RouteCallbackFunction;
	typedef std::unordered_map<std::string, RouteCallbackFunction> RoutCallBackHandlersMap;
	RoutCallBackHandlersMap m_RouteCallbackHandlers;
	HttpRouteCallBackHandler m_HttpRouteCallBackHandler;

	// Helper Macro for the Http Route Callbacks
    #define ADD_HTTP_ROUTE_CALLBACK(route,methodName) \
    m_RouteCallbackHandlers.emplace(route,std::bind(&HttpRouteCallBackHandler::methodName, std::ref(m_HttpRouteCallBackHandler), std::placeholders::_1))

	// Handles low level socket implementation
	std::unique_ptr<TcpSocketHandler>  m_tcpSocketHanderPtr;

	// Handles parsing and generating http streams
	HttpStreamHandler m_httpStreamHandler;

	// Main asynchronous thread which will get invoked whenever there is new request from client
	// And reads the client data,converts to user defined HttpRequest and will callback
	// appropriate registered callback handler and response will send back to the client
	void handleClientRequest(SOCKET clientSocket);
};
