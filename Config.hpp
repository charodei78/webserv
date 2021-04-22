#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>

class Config
{
    public:
        Config();
        ~Config();
        Config &operator=(Config const &);
        std::string host;
        int port;
        int clientLimit; 
        std::string routFoulder;
        std::map<std::string, std::string> metaVariables;
};

#endif