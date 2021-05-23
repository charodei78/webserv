//
// Created by Harren Leilani on 5/23/21.
//

#include "Client.hpp"

Client::Client(int sock) : sock(sock)
{
    currentState = requestParsing;
}

int Client::getSock() {
    return sock;
}

int Client::readRequest() {
    time(&lastOperationTime);
}

int Client::sendResponse() {
    time(&lastOperationTime);
}
