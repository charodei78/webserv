//
// Created by Harren Leilani on 5/1/21.
//

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "includes.hpp"
#include <list>
#include "Config.hpp"

using namespace std;

class ConfigParser
{
public:
    ConfigParser();
    ~ConfigParser();

    list<Config> GetServerConfigs(const string &configFileName);

private:
    int iter;
    vector<string> configLines;
    string GetStringFromFile(string fileName);
    vector<string> GetKeyValue(string line);
    Config ConfigParse();
    void ParseOptionIntoConfig(Config &config);
    void ParseLocationIntoConfig(Config &config, vector<string> &words);
    void ParseWordsInMap(vector<string> words, map<string, string> &map, int i);
    void ParseError();
};


#endif
