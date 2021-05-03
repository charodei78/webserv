#include "Config.hpp"

Config::Config() 
{
    cgiPath = "../cgi/a.out";
    port = 80;
    domain = "localhost";
    ip = "127.0.0.1";
    limitClientBodySize = 1024;
    rootDirectory = "./";
}

Config::~Config(){}

void Config::ParseMetaVariables()
{
    map<string, string> unusedOptions(metaVariables);

    parseFieldFromMap(unusedOptions, "index", index);
    parseFieldFromMap(unusedOptions, "server_name", domain);
    parseFieldFromMap(unusedOptions, "cgi_path", cgiPath);
    parseFieldFromMap(unusedOptions, "error_page", errorPage);
    parseFieldFromMap(unusedOptions, "allowed_functions", allowedFunctions);
    parseFieldFromMap(unusedOptions, "root", rootDirectory);
    parseFieldFromMap(unusedOptions, "listen", port);
    parseFieldFromMap(unusedOptions, "client_max_body_size", limitClientBodySize);

    if (unusedOptions.size() > 0)
    {
        map<string, string>::iterator  iter = unusedOptions.begin();
        while (iter != unusedOptions.end())
        {
            cerr << "Unknown field: " << iter->first << endl;
            iter++;
        }
        throw exception();
    }
}

//возвращает, нашел ли он это поле, если он его нашел, но его не получилось запарсить, выкинет exception
//если все ок, удалит его из этой мапы. подразумевается, что мапа хранит в себе незапаршенные значения
bool Config::parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, string &fieldRef)
{
    if (fieldMap.count(fieldKey) == 0)
        return false;

    string value = fieldMap[fieldKey];

    fieldRef = value;
    fieldMap.erase(fieldKey);
    return true;
}

bool Config::parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, int &fieldRef)
{
    if (fieldMap.count(fieldKey) == 0)
        return false;

    string value = fieldMap[fieldKey];

    try
    {
        fieldRef = stoi(value);
    }
    catch (exception)
    {
        return false;
    }
    fieldMap.erase(fieldKey);
    return true;
}

Config &Config::operator=(Config const &config)
{
	if (this != &config) {
		port = config.port;
        limitClientBodySize = config.port;
		ip = config.ip;
		domain = config.domain;
		metaVariables = config.metaVariables;
	}
    return *this;
}