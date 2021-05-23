//
// Created by Harren Leilani on 5/3/21.
//

#include "ServerEnvironment.hpp"

bool ServerEnvironment::RunServers(list<Config> &serverConfigs)
{
    if (serverListeners.size() > 0)
    {
        cerr << "Server already started\n";
        exit(1);
    }

    if (serverConfigs.size() == 0)
        return false;

    list<Config>::iterator  configIter = serverConfigs.begin();
    while (configIter != serverConfigs.end())
    {
        Server newServer(*configIter);

        vector<ServerListener>::iterator listenerIter = serverListeners.begin();

        while (listenerIter != serverListeners.end())
        {
            if (listenerIter->getPort() == configIter->port)
                break;
            listenerIter++;
        }

        if (listenerIter == serverListeners.end())
        {
            ServerListener newListener(configIter->port);
            newListener.BindServer(newServer);
            serverListeners.push_back(newListener);
        }
        else
            listenerIter->BindServer(newServer);
        configIter++;
    }

    fd_set readMasterSet;
    fd_set writeMasterSet;

    max_fd = 0;

    vector<ServerListener>::iterator listenerIter = serverListeners.begin();
    while (listenerIter != serverListeners.end()) {
        if (!listenerIter->Intialize()) {
            exit(1);
        }
        FD_SET(listenerIter->getSock(), &readMasterSet);
        if (listenerIter->getSock() > max_fd)
            max_fd = listenerIter->getSock();
        listenerIter++;
    }

    timeval timeoutMaster;
    timeoutMaster.tv_usec = 0;
    timeoutMaster.tv_sec = 2;

    vector<Client>::iterator clientIter;
    while (true)
    {
        timeval timeout = timeoutMaster;
        fd_set readSet;
        fd_set writeSet;
        FD_COPY(&readMasterSet, &readSet);
        FD_COPY(&writeMasterSet, &writeSet);

        int ret = select(max_fd + 1, &readSet, &writeSet, nullptr, &timeout);

        if (!ret)
            continue;

        //Проход по каждому серверу
        listenerIter = serverListeners.begin();
        while (listenerIter != serverListeners.end()) {
            //Есть ли новый клиент
            if (FD_ISSET(listenerIter->getSock(), &readSet))
            {
                try
                {
                    Client newClient = listenerIter->acceptClient();

                    if (newClient.getSock() > max_fd)
                        max_fd = newClient.getSock();

                    listenerIter->clients.push_back(newClient);
                    FD_SET(newClient.getSock(), &readMasterSet);
                }
                catch (exception)
                {
                    cerr << "Accept err: " << strerror(errno) << endl;
                }
            }

            //Проход по каждому клиенту этого сервака
            clientIter = listenerIter->clients.begin();
            while (clientIter != listenerIter->clients.end())
            {
                //Нужно ли читать и есть ли что читать
                if (clientIter->currentState == requestParsing && FD_ISSET(clientIter->getSock(), &readSet))
                {
                    if (clientIter->readRequest() <= 0){
                        //Закрыто соединение
                        FD_CLR(clientIter->getSock(), &readMasterSet);
                        listenerIter->clients.erase(clientIter);
                        close(clientIter->getSock());
                    }

                    if (clientIter->currentState == sendingResponse)
                    {
                        FD_CLR(clientIter->getSock(), &readMasterSet);
                        FD_SET(clientIter->getSock(), &writeMasterSet);
                    }
                }

                //Запись, если есть куда писать и нужно ли ему писать
                if (clientIter->currentState == sendingResponse && FD_ISSET(clientIter->getSock(), &writeSet))
                {
                    if (clientIter->sendResponse() <= 0)
                    {
                        //Закрылось соединение
                        FD_CLR(clientIter->getSock(), &writeMasterSet);
                        listenerIter->clients.erase(clientIter);
                        close(clientIter->getSock());
                    }

                    if (clientIter->currentState == closeConnection)
                    {
                        FD_CLR(clientIter->getSock(), &writeMasterSet);
                        listenerIter->clients.erase(clientIter);
                        close(clientIter->getSock());
                    }
                }

                clientIter++;
            }

            listenerIter++;
        }
    }
}
