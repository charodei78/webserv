 #include "Server.hpp"
 #include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Helper.hpp"
#include "Server.hpp"
#include "ServerEnvironment.hpp"
#include "ConfigParser.hpp"
#include "ServerListener.hpp"

int main()
{


     ConfigParser parser;

     list<Config> testConfigs = parser.GetServerConfigs("../test.conf");

    ServerEnvironment env;
    env.RunServers(testConfigs);
}