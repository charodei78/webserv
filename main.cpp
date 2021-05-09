#include "ConfigParser.hpp"
#include "ServerEnvironment.hpp"

int main()
{
//
//	int fd = open("../tmp/tmp_in.txt", O_RDONLY);
//
//	readBefore(fd, "\r\n\r\n", 1024);
//	cout << readLine(fd);

//    sockaddr_in r;
//    SendAuthorizationRequest(r, 0);

     ConfigParser parser;

        list<Config> testConfigs = parser.GetServerConfigs("../test2.conf");
        ServerEnvironment env;
        env.RunServers(testConfigs);
}