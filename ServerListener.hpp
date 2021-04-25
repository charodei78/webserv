#ifndef SERVERLISTENER_CPP
#define SERVERLISTENER_CPP

#include <list>
#include "Server.hpp"

class ServerListener
{
    public:
        ServerListener(const int port);
        ~ServerListener();
        ServerListener operator=(ServerListener&);
        void ProcessConectionToServer(sockaddr_in client_addr, int client_socket);
        bool Intialize(); //Привязывается
        void StartListen(); //Начинает прослушивать свой порт
        bool BindServer(Server& server); //Добавляет сервер в лист, на который может быть отправлено соединение
    private:
        int sock;
        const int port;
        ServerListener();
        std::list<Server> servers;
};

#endif