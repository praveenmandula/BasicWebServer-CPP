/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#include "BasicWebServer.h"

int main(int argc, char* argv[]) {

    // Check if the number of arguments are correct
    if (argc != 2) {
        LogMessage(LogLevel::ERROR_R, " Invalid arguments passed,Correct Usage as below ");
        LogMessage(LogLevel::ERROR_R, " 'PortNumber' ");
        return 1; // Return non-zero to indicate an error
    }

    // Access and use the arguments
    int tcp_server_port = atoi(argv[1]);

    // Print out the arguments
    LogMessage(LogLevel::INFO, " Provided Server PORT : ", tcp_server_port);

    // Create Basic Webserver instance with specified port
    BasicWebServer server(tcp_server_port);

    // Register Callbacks
    // Here we need to register our callback method for the routes required
    server.registerHttpRouteCallbacks();

    // Accept Connections -> WebServer will be waiting on loop here
    // Client incoming request will get parse and appropriate callback gets called
    // And sends back responses to the client
    server.acceptConnections();

    return 0;
}
