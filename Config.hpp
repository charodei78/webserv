#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include "Location.hpp"
#include <vector>
#include <iostream>

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
        string cgiPath;
        string index;
        string errorPage;
        string allowedFunctions; //example: GET/POST , POST/GET, GET, POST
        int port;
        int limitClientBodySize;
        string rootDirectory;
        map<string, string> metaVariables;
        vector<Location> locations;
        void ParseMetaVariables();
    private:
        bool parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, string &fieldRef);
        bool parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, int &fieldRef);
};

#endif