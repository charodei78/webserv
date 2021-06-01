#include "Server/Config/ConfigParser.hpp"
#include "Server/ServerCluster.hpp"
#include "Helpers/Helper.hpp"
#include "ENV.h"

int main(int argc, char **argv)
{

	signal(SIGPIPE, SIG_IGN);

	ConfigParser parser;

	string config = "../conf/eval.conf";
	cout << argc;
	(void)argv;
	try {
	    list<Config> testConfigs = parser.GetServerConfigs("../conf/eval.conf");
	    ServerCluster env(testConfigs);
	    env.RunServers();
	} catch (exception &e) {
	    cerr << e.what() ;
	}


}