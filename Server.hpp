#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"
#include "Location.hpp"
#include "Response.hpp"
#include "includes.hpp"

using namespace std;

class Server
{
    private:
        std::string serverName;
        map<Location, Server> locations;
    public:
        Config serverConfig;

	    Server();
	    Server(Config server_config);
        Server(Server const &rhs);
        Server &operator=(Server const &rhs);
        bool operator==(Server);
        ~Server();
        Server *GetServerLocation(string path);
        Server &GetLocationServer(const string &uri);
        string GetServerName();
        void SendAuthorizationRequest(const sockaddr_in &addr, const int sock);
        bool SendHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request, Config *config);
        static void printLog(sockaddr_in client_addr, const string& message);
};

#endif