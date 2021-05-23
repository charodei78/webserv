#ifndef SERVERLISTENER_CPP
#define SERVERLISTENER_CPP

#include <list>
#include "Server.hpp"
#include "../Helpers/Reader.hpp"
#include "sys/stat.h"
#include "Client.hpp"

static int max_fd;

class ServerListener
{
    public:
        ServerListener(const int port);
        ~ServerListener();
        ServerListener operator=(ServerListener&);
        void ProcessConnectionToServer(sockaddr_in client_addr, int client_socket);
        int getPort() const;
        bool Intialize(); //Привязывается
        void StartListen(); //Начинает прослушивать свой порт
        int getSock();
        bool BindServer(Server& server); //Добавляет сервер в лист, на который может быть отправлено соединение
        Client &acceptClient();
        vector<Client> clients;
private:
    int sock;
    const int port;
        static void printLog(sockaddr_in client_addr, const string &message);
        Server &FindServerByHost(std::string host);
        ServerListener();
        std::list<Server> servers;
};

#endif