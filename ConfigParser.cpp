//
// Created by Harren Leilani on 5/1/21.
//

#include "ConfigParser.hpp"


ConfigParser::ConfigParser() {
}

ConfigParser::~ConfigParser() {}

void ConfigParser::ParseError()
{
    cerr << "Invalid config: " << iter + 1 << " line";
    exit(1);
}

void ConfigParser::ParseWordsInMap(vector<string> words, map<string, string> &map, int i)
{
    if (*(words[1].end() - 1) != ';')
        ParseError();
    words[1].erase(words[1].end() - 1);
    map[words[0]] = words[1];
}

vector<string> ConfigParser::GetKeyValue(string line)
{
    vector<string>  words;
    string::iterator endFirstWord = find_if(line.begin(), line.end(), isEmptySpace);
    words.push_back(string(line.begin(), endFirstWord));
    words.push_back(string(find_if_not(endFirstWord, line.end(), isEmptySpace), line.end()));
    return words;
}

void ConfigParser::ParseLocationIntoConfig(Config &config, vector<string> &words)
{
    string line = trim(configLines[iter]);

    if (*words[1].begin() != '/')
        ParseError();
    Location newLocation(words[1]);

    iter++;

    while (trim(configLines[iter]) == "") // скипаем пустые строки после server
        iter++;

    if (trim(configLines[iter]) != "{") // проверяем открывается ли фигурная скобка после сервер, или там какой-то мусор
        ParseError();
    else
        iter++;

    while (line != "}")
    {
        if (line != "")
        {
            string line = trim(configLines[iter]);

            vector<string> words = GetKeyValue(line);

            ParseWordsInMap(words, newLocation.metaVariables, iter);
        }
        iter++;
        line = trim(configLines[iter]);
    }

    config.locations.push_back(newLocation);
}

void ConfigParser::ParseOptionIntoConfig(Config &config)
{
    string line = trim(configLines[iter]);

    vector<string> words = GetKeyValue(line);

    if (words.size() != 2)
        ParseError();

    if (isContain(words[1], isEmptySpace))
        ParseError();

    if (words[0] != "location") //парсим просто метаданные
    {
        ParseWordsInMap(words, config.metaVariables, iter);
    }
    else //парсим локацию
    {
        ParseLocationIntoConfig(config, words);
    }
}

Config ConfigParser::ConfigParse()
{
    while (trim(configLines[iter]) == "") // скипаем пустые строки после server
        iter++;

    if (trim(configLines[iter]) != "{") // проверяем открывается ли фигурная скобка после сервер, или там какой-то мусор
        ParseError();
    else
        iter++;

    Config newConfig;
    string line = trim(configLines[iter]);
    while (line != "}")
    {
        if (line != "")
            ParseOptionIntoConfig(newConfig);
        iter++;
        line = trim(configLines[iter]);
    }
    iter++;
    return newConfig;
}

list<Config> ConfigParser::GetServerConfigs(const string &configFileName)
{
    list<Config> configs;
    string fileContent = file_get_contents(configFileName);
    configLines = split('\n', fileContent);

    for (iter = 0; iter < configLines.size(); iter++)
    {
        string line = trim(configLines[iter]);
        if (line == "server") //находим сервер => добавляем его, функция парсит его
        {
            iter++;
            configs.push_back(ConfigParse());
        }
        else if (line == "") //скипаем пустые строчки
            continue;
        else
            ParseError();
    }

    list<Config>::iterator  iter = configs.begin();
    while (iter != configs.end())
    {
        try
        {
            iter->ParseMetaVariables();
        }
        catch (exception)
        {
            cerr << "Invalid server fields";
            exit(1);
        }
        iter++;
    }

    return configs;
}