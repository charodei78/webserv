#include "Server/Config/ConfigParser.hpp"
#include "Server/ServerCluster.hpp"
#include "Helpers/Helper.hpp"
#include "ENV.h"

int main(int argc, char **argv)
{

	signal(SIGPIPE, SIG_IGN);

	ConfigParser parser;

	string config = "./conf/eval.conf";
	if (argc > 2)
	{
		cerr << "Invlalid parameters\n";
		return 1;
	}
	if (argc == 2)
		config = argv[1];
	try {
	    list<Config> testConfigs = parser.GetServerConfigs(config);
	    ServerCluster env(testConfigs);
	    env.RunServers();
	} catch (exception &e) {
	    cerr << e.what() ;
	}


}