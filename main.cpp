 #include "Server.hpp"
 #include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Helper.hpp"
#include "Server.hpp"

int main()
{
    Config config;
    config.port = 80;
    config.clientLimit = 5;
    config.ip = "127.0.0.1";
    config.domain = "localhost";
    config.cgi_path = "../cgi/cgi_tester";
    config.root_directory = "../public";
    Server server1(config);
    if (server1.Intialize())
        server1.StartListening();
}