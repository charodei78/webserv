//
// Created by Harren Leilani on 5/1/21.
//

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
