//
// Created by Harren Leilani on 5/23/21.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP


#define OPERATION_BYTE_SIZE 32768

#include "Server.hpp"

enum state
        {
    requestParsing,
    sendingResponse,
    closeConnection
        };

using namespace std;

class Client {
private:
    Server attachedServer;
    const int sock;
    string readBuffer;
    string sendBuffer;
public:
    state currentState;
    Client(int sock);
    int getSock();
    int readRequest();
    int sendResponse();
};


#endif //WEBSERV_CLIENT_HPP
