//
// Created by Harren Leilani on 5/1/21.
//

#include <vector>
#include "Helper.hpp"
#include "Location.hpp"

Location::Location(std::string path) : path(path) {}

Location::~Location(){}

Location &Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->path = rhs.path;
	}
	return *this;
}


bool Location::operator<(const Location rhs) const  {
    return this->path < rhs.path;
}


bool Location::IsUriValid(const string &uri) const {
    string avaliablePath = path;

    if (avaliablePath[0] == '*' && avaliablePath[1] == '.')
    {
        string avaliableExtension = string(avaliablePath, 2);
        vector<string> uriParts = split("/", uri);
        if (uriParts.size() == 0)
            return false;
        string lastWord = *(uriParts.end() - 1);
        string::reverse_iterator dot = find(lastWord.rbegin(), lastWord.rend(), '.');
        return (string(lastWord.rbegin(), dot) == avaliableExtension);
    }
    if (avaliablePath[0] != '/')
        return false;

    if (uri == avaliablePath)
        return true;

    vector<string> uriParts = split("/", uri);
    vector<string> pathParts = split("/", avaliablePath);
    vector<string>::iterator pathIter = pathParts.begin();
    vector<string>::iterator  uriIter = uriParts.begin();
    while (pathIter != pathParts.end())
    {
        if (*pathIter == "*")
            return true;
        else if (*pathIter != *uriIter || uriIter == uriParts.end())
            return false;
        pathIter++;
        uriIter++;
    }

    if (uriIter != uriParts.end() || *(avaliablePath.end() - 1) != *(uri.end() - 1))
        return false;

    return true;
}
