#include "Config.hpp"

Config::Config() 
{
    port = 80;
    host = "local";
    clientLimit = 5;
    root_directory = "/app";
}

Config::~Config(){}

Config &Config::operator=(Config const &config)
{
    port = config.port;
    clientLimit = config.port;
    host = config.host;
    metaVariables = config.metaVariables;
    return *this;
}