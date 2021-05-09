#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include "Location.hpp"
#include <vector>
#include <iostream>

using namespace std;

static string valibleMetaVariables[10] = {
		"autoindex",
        "index",
        "error_page",
        "client_max_body_size"};

class Config
{
    public:
		string              domain;
		string              ip;
		string              cgiPath;
		string              index;
		string              errorPage;
		string              allowedFunctions; //example: GET/POST , POST/GET, GET, POST
		int                 port;
		int                 limitClientBodySize;
		bool                auth;
		string              auth_file_path;
		vector<string>      auth_file_content;
		string              rootDirectory;
		map<string, string> metaVariables;
		vector<Location>    locations;

		Config();
		~Config();
		Config &operator=(Config const &rhs);
	    void ParseMetaVariables();

	    string getIndexPath(string path);

	bool isCGI(string const &basicString);

private:
        bool parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, string &fieldRef);
        bool parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, int &fieldRef);
        bool parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, bool &fieldRef);
};

#endif