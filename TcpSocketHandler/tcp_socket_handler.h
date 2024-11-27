#pragma once
#include "cpp_standard_includes.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#define BUFFER_SIZE 4096

class TcpSocketHandler {
public:
	TcpSocketHandler() = delete;
	TcpSocketHandler(int port);
	SOCKET acceptConnections();

	int sendDataOnSocket(SOCKET& socket, std::string& message);
	int recvDataOnSocket(SOCKET& socket, std::string& message);

	int closeSocket(SOCKET closeSocket = INVALID_SOCKET);

public:
	~TcpSocketHandler();

private:
	int createSocket();
	int bindSocket();
	int listenSocket();
	

	SOCKET m_server_socket = INVALID_SOCKET;
	sockaddr_in server_address{};
	int m_port;
};