//
// Created by Harren Leilani on 5/1/21.
//

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <string>

//#include "Server.hpp"

class Config;

using namespace std;

class Location
{
public:
    string path;
    map<string, string> metaVariables;
    Config *config;
    Location(string path);
    Location &operator=(const Location & rhs);
    bool IsUriValid(const string &uri) const;
    bool operator<(const Location rhs) const;
    ~Location();

    bool IsUriValid(const string uri);
};

//#include "Server.hpp"

#endif