#include "Helper.hpp"
#include "Config.hpp"

Config::Config() 
{
    auth = false;
    auth_file_path = "../.htpasswd";
    cgiPath = "../cgi/a.out";
    port = 80;
    index = "index.html";
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
    parseFieldFromMap(unusedOptions, "basic_auth_file", auth_file_path);
    parseFieldFromMap(unusedOptions, "basic_auth", auth);

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
    if (auth)
    {
        string auth_file;
        try
        {
            auth_file = file_get_contents(auth_file_path);
        }
        catch (exception)
        {
            cerr << "Invalid auth file\n";
            throw exception();
        }
        auth_file_content = split("\n", auth_file);
    }

//    vector<Location>::iterator locationBegin = locations.begin();
//    while (locationBegin != locations.end())
//    {
//        Location &location = *locationBegin;
//        Config configCopy;
//        configCopy = *this;
//        configCopy.metaVariables = location.metaVariables;
//        configCopy.ParseMetaVariables();
//        location.server = new Server(configCopy);
//        locationBegin++;
//    }
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

	if (fieldKey == "client_max_body_size") {
		if (value.find('k') != -1)
			fieldRef *= 1024;
		else if (value.find('M'))
			fieldRef *= 1048576;
	}

    fieldMap.erase(fieldKey);
    return true;
}

bool Config::parseFieldFromMap(map<string, string> &fieldMap, string fieldKey, bool &fieldRef)
{
    if (fieldMap.count(fieldKey) == 0)
        return false;

    string value = fieldMap[fieldKey];

    if (value == "on")
    {
        fieldRef = true;
    }
    else if (value == "off")
        fieldRef = false;
    else
        return false;

    fieldMap.erase(fieldKey);
    return true;
}

Config &Config::operator=(Config const &rhs)
{
	if (this != &rhs) {
		this->domain = rhs.domain;
		this->ip = rhs.ip;
		this->cgiPath = rhs.cgiPath;
		this->index = rhs.index;
		this->errorPage = rhs.errorPage;
		this->allowedFunctions = rhs.allowedFunctions;
		this->port = rhs.port;
		this->limitClientBodySize = rhs.limitClientBodySize;
		this->rootDirectory = rhs.rootDirectory;
		this->metaVariables = rhs.metaVariables;
		this->locations = rhs.locations;
		this->auth = rhs.auth;
		this->auth_file_content = rhs.auth_file_content;
		this->auth_file_path = rhs.auth_file_path;
	}
    return *this;
}

string Config::getIndexPath(string path)
{
	string absPath;
	vector<string> indexes = split(" ", index);

	string root = rootDirectory + path;

	if (!exists(root))
		return "";
	if (is_file(root))
		return root;

	if (*(--path.end()) != '/')
		root += '/';
	
	for (int i = 0; i < indexes.size(); i++)
	{
		absPath = root + indexes[i];
		if (exists(absPath))
			break;
		else
			absPath = "";
	}
	if (absPath.empty() && exists(root))
		return root;
	return absPath;
}

bool Config::isCGI(const string &basicString)
{
	return false;
}


