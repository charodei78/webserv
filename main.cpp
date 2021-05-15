#include "ConfigParser.hpp"
#include "ServerEnvironment.hpp"

int main(int argc, char **argv)
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

//    if (argc > 1)
//    {
//        cerr << "Arg err";
//        exit(1);
//    }
//
//    string confFile = "../test2.conf";
//    if (argc == 1)
//    {
//        confFile = argv[1];
//    }

     ConfigParser parser;

        list<Config> testConfigs = parser.GetServerConfigs("../eval.conf");
        ServerEnvironment env;
        env.RunServers(testConfigs);

}