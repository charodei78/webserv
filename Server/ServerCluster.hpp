//
// Created by Harren Leilani on 5/3/21.
//

#ifndef WEBSERV_SERVERENVIRONMENT_H
#define WEBSERV_SERVERENVIRONMENT_H

#include "Config/Config.hpp"
#include "ServerListener.hpp"
#include <vector>

using namespace std;

#define OPERATION_TIMEOUT 10

class ServerCluster
{
public:
    bool RunServers();
    explicit ServerCluster(list<Config> &serverConfigs);
private:
    void intializeServerListeners();
    void closeClientConnection(ServerListener &listener, vector<Client>::iterator &clientIter);
    vector<ServerListener> serverListeners;
    fd_set readMasterSet;
    fd_set writeMasterSet;
    timeval timeoutMaster;
    list<Config> serverConfigs;
};


#endif
