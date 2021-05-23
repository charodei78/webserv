#ifndef SERVERLISTENER_CPP
#define SERVERLISTENER_CPP

#include <list>
#include "Server.hpp"
#include "../Helpers/Reader.hpp"
#include "sys/stat.h"
#include "Client.hpp"

static int max_fd;

class Client;

class ServerListener
{
    public:
        ServerListener(const int port);
        ~ServerListener();
        ServerListener operator=(ServerListener&);
        int getPort() const; //Порт на котором прослушивается соединение
        bool Intialize(); //Привязывается и занимает порт
        int getSock();
        bool BindServer(Server& server); //Добавляет сервер в лист, на который может быть отправлено соединение
        Client acceptClient(); //Принять клиента, который стучится на этот сервер
        vector<Client> clients; //Список клиентов подключенных к этому серверу
        Server &FindServerByHost(std::string host); //Найти определенный сервер по servername
private:
    int sock;
    const int port;
    static void printLog(sockaddr_in client_addr, const string &message);
        ServerListener();
        std::list<Server> servers;
};

#endif