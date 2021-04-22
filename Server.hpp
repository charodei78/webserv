#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "Config.hpp"
#include "Response.hpp"

class Server
{
    public:
        Server(const Config server_config);
        Server(Server const &rhs);
        Server &operator=(Server const &rhs);
        ~Server();

        bool Intialize();
        void StartListening();
        bool SendHttpResponse(const Http::Response);
    private:
        void ProcessConnection(const sockaddr_in &addr, const int sock);
        Server();
        int socket_fd;
        const Config serverConfig;
};

#endif