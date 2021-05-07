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
//
//	int fd = open("../tmp/tmp_in.txt", O_RDONLY);
//
//	readBefore(fd, "\r\n\r\n", 1024);
//	cout << readLine(fd);

     ConfigParser parser;

     list<Config> testConfigs = parser.GetServerConfigs("../test2.conf");

    ServerEnvironment env;
    env.RunServers(testConfigs);
}