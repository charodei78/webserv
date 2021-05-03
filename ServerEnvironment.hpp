//
// Created by Harren Leilani on 5/3/21.
//

#ifndef WEBSERV_SERVERENVIRONMENT_H
#define WEBSERV_SERVERENVIRONMENT_H

#include "Config.hpp"
#include "ServerListener.hpp"
#include <vector>

using namespace std;


class ServerEnvironment
{
public:
    bool RunServers(list<Config> &serverConfigs);

private:
    vector<ServerListener> serverListeners;
};


#endif
