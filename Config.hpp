#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include "Location.hpp"
#include <vector>

using namespace std;

static string valibleMetaVariables[10] = {
        "index",
        "error_page",
        "client_max_body_size"};

class Config
{
    public:
        Config();
        ~Config();
        Config &operator=(Config const &);
        string domain;
        string ip;
        string cgi_path;
        string protocol;
        int port;
        int clientLimit; 
        string root_directory;
        map<string, string> metaVariables;
        vector<Location> locations;
        void ParseMetaVariables();
};

#endif