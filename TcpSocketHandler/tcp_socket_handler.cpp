/*
 * Author: Praveen Mandula
 * Date: 11/18/2024
 * Description : TcpSocketHandler class is used for low level socket communication using windows/linux api's
 */
#include "tcp_socket_handler.h"

// public methods
TcpSocketHandler::TcpSocketHandler(int port)
{
	m_port = port;

    // socket initialization
	createSocket(); 
    bindSocket(); 
    listenSocket();
}

SOCKET TcpSocketHandler::acceptConnections()
{
    sockaddr_in client_address;
    socklen_t client_addr_size = sizeof(client_address);

    SOCKET client_socket = accept(m_server_socket, (struct sockaddr*)&client_address, &client_addr_size);
    if (client_socket == INVALID_SOCKET) {
        LogMessage(LogLevel::ERROR_R, "Accepting client connection failed");
        return INVALID_SOCKET;
    }
    // Print client received Address 
    //LogMessage(LogLevel::DEBUG, "Requested Client Address : : ", inet_ntop(client_address.sin_addr));

    return client_socket;
}

int TcpSocketHandler::sendDataOnSocket(SOCKET& socket, std::string& message)
{
    return send(socket, message.c_str(), (int)message.length(), 0);
}

int TcpSocketHandler::recvDataOnSocket(SOCKET& socket,std::string& message)
{
    char buffer[BUFFER_SIZE] = {};
    // Receive data from client
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        // Receive a chunk of data
        auto valread = recv(socket, buffer, BUFFER_SIZE, 0);
        if (valread == 0) {
            LogMessage(LogLevel::ERROR_R, "Connection closed by server");
            // Connection closed by server
            break;
        }
        else if (valread < 0) {
            // Error during recv
            LogMessage(LogLevel::ERROR_R, "recv error - unable to receive data on socket");
            break;
        }
        else {
            // Append received data to the message
            message.append(buffer, valread);
            // Check if all data has been received
            if (valread < BUFFER_SIZE) {
                LogMessage(LogLevel::INFO, "Received complete data from server, length =",message.length());
                // All data received
                break;
            }
        }
    }

    LogMessage(LogLevel::DEBUG, " Client data received = ", message);
    return 0;
}

// private methods
int TcpSocketHandler::createSocket()
{
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        LogMessage(LogLevel::ERROR_R, "Error: WSAStartup failed");
        return 1;
    }
#endif
    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket == INVALID_SOCKET) {
        LogMessage(LogLevel::ERROR_R, "Error: Could not create socket");
        return 1;
    }
    return 0;
}

int TcpSocketHandler::bindSocket()
{
    server_address.sin_family        =   AF_INET;
    server_address.sin_addr.s_addr   =   INADDR_ANY; // Accept connections from any IP
    server_address.sin_port          =   htons(m_port); // Port number

    if (bind(m_server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        LogMessage(LogLevel::ERROR_R, "Error: Bind failed");
        return 1;
    }
    return 0;
}

int TcpSocketHandler::listenSocket()
{
    if (listen(m_server_socket, 3) < 0) {
        LogMessage(LogLevel::ERROR_R, "Error: Listen failed");
        return 1;
    }
    return 0;
}

int TcpSocketHandler::closeSocket(SOCKET closeSocket)
{
    if (closeSocket != INVALID_SOCKET) {
        // Close socket and cleanup
#ifdef _WIN32
        closesocket(closeSocket);
#else
        close(closeSocket);
#endif
    }
    else {
        // Close socket and cleanup
#ifdef _WIN32
        closesocket(m_server_socket);
        WSACleanup();
#else
        close(m_server_socket);
#endif
    }
    return 0;
}

TcpSocketHandler::~TcpSocketHandler()
{
    closeSocket();
}
