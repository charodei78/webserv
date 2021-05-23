//
// Created by Harren Leilani on 5/23/21.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP


#define OPERATION_BYTE_SIZE 32768

#include "Server.hpp"
#include "../HTTP/Request.hpp"
#include "ctime"

enum state
        {
    requestParsing,
    sendingResponse,
    closeConnection
        };

using namespace std;

class Client {
private:
    Server attachedServer;
    const int sock;
    string readBuffer;
    string sendBuffer;
public:
    state currentState; //Как необходимо сейчас работать с этим клиентом
    time_t lastOperationTime; //Во избежании простаивания или зависания клиентов и их накопления запоминаем когда в последней раз они откликались

    explicit Client(int sock);
    int getSock();
    int readRequest(); //Чтение запроса с сокета клиента (вызов = можно читать)
    int sendResponse(); //Отправка ответа клиенту (вызов = можно писать)
};


#endif //WEBSERV_CLIENT_HPP
