//
// Created by Holli Heimerdinger on 5/28/21.
//

#ifndef WEBSERV_REQUESTPARSER_HPP
#define WEBSERV_REQUESTPARSER_HPP


#include "../Helpers/Reader.hpp"
#include "ServerListener.hpp"
#include "Client.hpp"

class ServerListener;

class RequestParser
{

	RequestParser &operator=(RequestParser const &rhs);
	RequestParser(RequestParser const &rhs);

	int checkStatus(int status);
	int onError(int code);
	int parseChunked(int sock, int bodyLimit);

public:
	RequestParser(const sockaddr_in addr);
	~RequestParser();

	Reader              *reader;
	Http::Request       request;
	Http::Response      response;


	const sockaddr_in   addr;
	int                 bodyFd;
	Config              *config;
	Server              *server;

	int parse(int sock, ServerListener &listener);

};


#endif //WEBSERV_REQUESTPARSER_HPP
