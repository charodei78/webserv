#include "Config.hpp"

Config::Config() 
{
    protocol = "http";
    domain = "localhost";
    ip = "127.0.0.1";
    clientLimit = 5;
    root_directory = "/app";
}

Config::~Config(){}

void Config::ParseMetaVariables()
{
    map<string, string> unusedOptions(metaVariables);


}

Config &Config::operator=(Config const &config)
{
	if (this != &config) {
		port = config.port;
		clientLimit = config.port;
		ip = config.ip;
		domain = config.domain;
		metaVariables = config.metaVariables;
	}
    return *this;
}