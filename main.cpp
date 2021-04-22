#include "Server.hpp"

int main()
{
    Config config;
    config.port = 1500;
    config.clientLimit = 5;
    config.host = "127.0.0.1";
    Server server1(config);
    if (server1.Intialize())
        server1.StartListening();
}