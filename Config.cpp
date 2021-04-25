#include "Config.hpp"

Config::Config() 
{
    port = 80;
    protocol = "http";
    domain = "localhost";
    ip = "127.0.0.1";
    clientLimit = 5;
    root_directory = "/app";
}

Config::~Config(){}

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