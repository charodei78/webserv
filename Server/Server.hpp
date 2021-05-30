#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config/Config.hpp"
#include "Config/Location.hpp"
#include "../HTTP/Response.hpp"
#include "../includes.hpp"

using namespace std;

class Server
{
    private:
        std::string serverName;
        map<Location, Server> locations;
    public:
        Config serverConfig; //Привязанный конфиг

	    Server();
	    Server(Config server_config);
        Server(Server const &rhs);
        Server &operator=(Server const &rhs);
        bool operator==(Server);
        ~Server();
        Server &GetLocationServer(string &uri); //если есть локейшины, попробует найти его по uri
        string GetServerName(); //alternative: host
        void SendAuthorizationRequest(const sockaddr_in &addr, const int sock); //TODO: заменить
        Http::Response getHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request, Config *config); //TODO: заменить
        static void printLog(sockaddr_in client_addr, const string& message);
};

#endif