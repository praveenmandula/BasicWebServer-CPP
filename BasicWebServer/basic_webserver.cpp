/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#include "basic_webserver.h"

BasicWebServer::BasicWebServer(int port)
{
    m_tcpSocketHanderPtr = std::make_unique<TcpSocketHandler>(port);
}

void BasicWebServer::handleClientRequest(SOCKET clientSocket)
{
    // GetData from the client using client socket
    std::string received_data;
    m_tcpSocketHanderPtr->recvDataOnSocket(clientSocket, received_data);

    LogMessage(LogLevel::DEBUG, "Received client data = ", received_data);

    // parseHttpRequest - will parse raw client data into user defined HttpRequest structure
    HttpStreamHandler::HttpRequest  request = m_httpStreamHandler.parseHttpRequest(received_data);
    HttpStreamHandler::HttpResponse response;

    // find and call callback handler based on requested route from the client
    auto it = m_RouteCallbackHandlers.find(request.path);
    if (it != m_RouteCallbackHandlers.end()) {
        response = it->second(request);
    }
    else {
        response.statusCode = HttpStreamHandler::httpStatusCodes::NOT_FOUND;
        response.statusMessage = "Failed to find the required callback handler";
        std::string bodyMessage = "Request url = " + request.path + " not implemented yet";
        response.body = bodyMessage;
    }

    // Converts user defined HttpResponse structure to the http response stream
    std::string httpResponseStream = m_httpStreamHandler.generateHTTPResponseStream(response);

    // Sends back http reponse to the client
    m_tcpSocketHanderPtr->sendDataOnSocket(clientSocket, httpResponseStream);

    // close the client socket
    m_tcpSocketHanderPtr->closeSocket(clientSocket);
}

int BasicWebServer::acceptConnections()
{
    //LogMessage(LogLevel::INFO, "Server listening on : ", m_tcpSocketHanderPtr->getServerPort());

    while (true)
    {
        // acceptConnectionsFromClients will return client socket when client gets connected
        SOCKET client_socket = m_tcpSocketHanderPtr->acceptConnections();

        // Launch the thread only if its not a invalid socket
        if (client_socket != INVALID_SOCKET) {

            // use any of the thread launcher
            std::thread clientThread(&BasicWebServer::handleClientRequest, this, client_socket);
            clientThread.detach();

            // asyn also does the same job, but we need to have future object associated to it
            //std::async(std::launch::async, &WebServer::handleClientRequest, this, client_socket);
        }
    }
    return 0;
}

void BasicWebServer::registerHttpRouteCallbacks()
{
    // Add Http Route Callback Routes, any additional route need to be added here
    ADD_HTTP_ROUTE_CALLBACK("/",      handleWelcomePage);
    ADD_HTTP_ROUTE_CALLBACK("/login", handleUserLogin);
    ADD_HTTP_ROUTE_CALLBACK("/registration", handleUserRegistration);
    ADD_HTTP_ROUTE_CALLBACK("/upload",handleFileUpload);
}
