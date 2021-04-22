#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "Config.hpp"
#include "Response.hpp"

class Server
{
private:
	int socket_fd;
	const Config serverConfig;
    Server();

public:
    Server(const Config server_config);
    Server(Server const &rhs);
    Server &operator=(Server const &rhs);
    ~Server();
	void startServer();
	void closeServer();

	_Noreturn void handle();

        bool Intialize();
        void StartListening();
        bool SendHttpResponse(const Http::Response);
        void ProcessConnection(const sockaddr_in &addr, const int sock);
};

#endif