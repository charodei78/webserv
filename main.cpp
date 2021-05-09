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

//	int fd = open("tmp/tmp_in.txt", O_RDONLY | O_CREAT);
//	cout << file_put_contents("../tete/../toto/tutu/test.txt", "test", 0);
//	f;
//	close(fd);



//
//	readBefore(fd, "\r\n\r\n", 1024);
//	cout << readLine(fd);

//    sockaddr_in r;
//    SendAuthorizationRequest(r, 0);

     ConfigParser parser;

     list<Config> testConfigs = parser.GetServerConfigs("../eval.conf");
    ServerEnvironment env;
    env.RunServers(testConfigs);
}