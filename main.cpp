 #include "Server.hpp"
 #include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Helper.hpp"
#include "Server.hpp"
#include "ConfigParser.hpp"
#include "ServerListener.hpp"

int main()
{
    
    Config config;
    config.port = 80;
    config.clientLimit = 5;
    config.ip = "127.0.0.1";
    config.domain = "localhost";
    config.cgi_path = "../cgi/cgi_tester";
    config.root_directory = "../public";

    Config newTestConfig = config;
    newTestConfig.domain = "test.localhost";
    newTestConfig.root_directory = "../test.public";

    ServerListener listener(80);
    
    Server server1(config);
    Server server2(newTestConfig);

    listener.BindServer(server1);
    listener.BindServer(server2);

    if (listener.Intialize())
        listener.StartListen();
    else
        std::cout << " initialize err";
    

    // ConfigParser parser;

    // list<Config> testConfigs = parser.GetServerConfigs("./test.conf");

    // std::cout << testConfigs.size();
}