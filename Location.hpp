//
// Created by Harren Leilani on 5/1/21.
//

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <string>

class Location
{
public:
    const std::string path;
    std::map<std::string, std::string> metaVariables;
    Location(std::string path);
    ~Location();
};

#endif