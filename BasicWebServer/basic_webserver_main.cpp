/*
 * Author: Praveen Mandula
 * Date: 2024-10-30
 */
#include "basic_webserver.h"

int main(int argc, char* argv[]) {

    // Check if the number of arguments are correct
    if (argc != 2) {
        LogMessage(LogLevel::ERROR_R, " Invalid arguments passed,Correct Usage as below ");
        LogMessage(LogLevel::ERROR_R, " 'PortNumber' ");
        return 1; // Return non-zero to indicate an error
    }

    // Access and use the arguments
    int tcp_server_port = atoi(argv[1]);

    // Create Basic Webserver instance with specified port
    BasicWebServer server(tcp_server_port);

    // Register Callbacks
    // Here we need to register our callback method for the http routes
    server.registerHttpRouteCallbacks();

    // Print Server listening on this port message
    LogMessage(LogLevel::INFO, "Server listening on PORT : ", tcp_server_port);

    // Accept Connections , BasicWebServer will be on Infinite loop here
    // Client incoming request will get parse and appropriate Callback will get called
    server.acceptConnections();

    return 0;
}
