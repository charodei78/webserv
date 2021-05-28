//
// Created by Harren Leilani on 5/23/21.
//

#include "Client.hpp"
#include "RequestParser.hpp"

Client::Client(int sock, sockaddr_in addr)
{
	currentState = requestParsing;
	this->sock = sock;
	this->requestParser = new RequestParser(addr);
	this->config = nullptr;
    this->server = nullptr;
    this->addr = addr;
    time(&lastOperationTime);
}

int Client::getSock() {
    return sock;
}


int Client::sendResponse() {
    time(&lastOperationTime);
    if (responseBuffer.empty())
        responseBuffer = response.toString();
    int sended = send(sock, responseBuffer.c_str(), responseBuffer.length(), 0);
    if (sended <= 0)
        return sended;
    responseBuffer = responseBuffer.erase(responseBuffer.size() - sended);
    if (responseBuffer.empty())
        currentState = closeConnection;
    return 1;
}


int Client::readRequest(ServerListener &listener)
{
	time(&lastOperationTime);
	usleep(1000);
	int ret = requestParser->parse(sock, listener);
	if (ret == -1)
	{
		perror("What");
		return -1;
	}
	if (ret == 2) {
		currentState = sendingResponse;
		response = requestParser->response;
	}
	return ret;
}

Client &Client::operator=(const Client &src) {
    if (src.sock == 0)
        return *this;
    if (this != &src) {
    	this->requestParser = new RequestParser(addr);
	    this->lastOperationTime = src.lastOperationTime;
        this->sock = src.sock;
        this->request = src.request;
        this->response = src.response;
        this->currentState = src.currentState;
        this->readBuffer = src.readBuffer;
        this->sendBuffer = src.sendBuffer;
        this->attachedServer = src.attachedServer;
        this->addr = src.addr;
    }
    return *this;
}

Client::Client(const Client &c) {
    *this = c;
}

Client::~Client() {
}
