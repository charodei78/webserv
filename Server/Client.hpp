//
// Created by Harren Leilani on 5/23/21.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP


#include "Server.hpp"
#include "RequestParser.hpp"
#include "../Helpers/Reader.hpp"
#include "../HTTP/Request.hpp"
#include "ctime"
#include "ServerListener.hpp"




class RequestParser;
class ServerListener;


using namespace std;

#ifndef READ_SIZE
#define READ_SIZE 32768
#endif

enum state
{
	requestParsing,
	sendingResponse,
	closeConnection
};

class Client {


private:
    Server attachedServer;
    sockaddr_in addr;
    int sock;
    string  readBuffer;
    string  sendBuffer;
	Config  *config;

	string responseBuffer = "";

//	Client();
public:
    state currentState;
    Server *server;
	Http::Request request;
	Http::Response response;
	RequestParser *requestParser;


    Client &operator=(const Client &src);
    Client(const Client&);
    ~Client();
    int readRequest(ServerListener &listener);
    Client(int sock, sockaddr_in addr);
    time_t lastOperationTime;
    int getSock();
    int sendResponse();


};


#endif //WEBSERV_CLIENT_HPP
