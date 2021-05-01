#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include "Location.hpp"
#include <vector>

class Config
{
    public:
        Config();
        ~Config();
        Config &operator=(Config const &);
        std::string domain;
        std::string ip;
        std::string cgi_path;
        std::string protocol;
        int port;
        int clientLimit; 
        std::string root_directory;
        std::map<std::string, std::string> metaVariables;
        std::vector<Location> locations;
};

#endif