//
// Created by Harren Leilani on 5/23/21.
//

#include "Client.hpp"

Client::Client(int sock, sockaddr_in addr)
{
	currentState = requestParsing;
	this->sock = sock;
	responseIsChunked = false;
	this->requestParser = new RequestParser(addr);
	this->config = nullptr;
    this->server = nullptr;
    this->addr = addr;
    time(&lastOperationTime);
}

// X-Secret-Header-For-Test: 1

int Client::getSock() {
    return sock;
}


int Client::sendResponse() {
    time(&lastOperationTime);


    if (responseBuffer.empty() && sendBuffer.empty())
    {
    	if (response.header("Transfer-Encoding") == "chunked")
	    {
		    responseIsChunked = true;
		    responseBuffer = response.body();
		    response.body("");
		    sendBuffer = response.toString();
	    } else {
		    sendBuffer = response.toString();
    	}
    }
    if (responseIsChunked && sendBuffer.empty())
    {
	    int message_size = std::min<int>(responseBuffer.length(), OPERATION_BYTE_SIZE);
	    sendBuffer = itoa(message_size, 16) + "\r\n" + responseBuffer.substr(0, message_size) + "\r\n";
	    responseBuffer.erase(0, message_size);
	    if (responseBuffer.empty())
		    sendBuffer += "0\r\n\r\n";
    }
    int sended = send(sock, sendBuffer.c_str(), sendBuffer.length(), 0);
    if (sendBuffer.length() < 20000)
		cout << sendBuffer << endl;

	cout << "sended " << sended << endl;
    if (sended <= 0)
        return sended;
	sendBuffer.erase(0, sended);
    if (responseBuffer.empty() && sendBuffer.empty())
        currentState = closeConnection;
    return 1;
}


int Client::readRequest(ServerListener &listener)
{
	time(&lastOperationTime);
	usleep(2000);
	int ret = requestParser->parse(sock, listener);
	if (ret == -1)
	{
		pError("What");
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
    	this->responseIsChunked = src.responseIsChunked;
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
