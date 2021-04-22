//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "includes.hpp"
#include "Request.hpp"

using namespace std;

class Server
{
	unsigned short port;
public:
	Server(unsigned port = 80);
	Server(Server const &rhs);
	~Server();
	void startServer();
	void closeServer();

	_Noreturn void handle();

	Server &operator=(Server const &rhs);

	private:
		int socket_fd;
};


#endif //WEBSERV_SERVER_HPP
