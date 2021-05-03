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

    list<Config>::iterator  configIterator = serverConfigs.begin();
    while (configIterator != serverConfigs.end())
    {
        Server newServer(*configIterator);

        vector<ServerListener>::iterator serverListenerIterator = serverListeners.begin();

        while (serverListenerIterator != serverListeners.end())
        {
            if (serverListenerIterator->getPort() == configIterator->port)
                break;
            serverListenerIterator++;
        }

        if (serverListenerIterator == serverListeners.end())
        {
            ServerListener newListener(configIterator->port);
            newListener.BindServer(newServer);
            serverListeners.push_back(newListener);
        }
        else
            serverListenerIterator->BindServer(newServer);
        configIterator++;
    }

    vector<ServerListener>::iterator listenersIterator = serverListeners.begin();
    while (listenersIterator != serverListeners.end()) {
        listenersIterator->Intialize();
        listenersIterator++;
    }


    for (;;)
    {
        listenersIterator = serverListeners.begin();
        while (listenersIterator != serverListeners.end()) {
            listenersIterator->StartListen();
            listenersIterator++;
        }
    }
    return true;
}
