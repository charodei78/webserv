//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef WEBSERV_HELPER_HPP
#define WEBSERV_HELPER_HPP

#include "includes.hpp"
#include <fstream> // TODO: #include <fstream>

using namespace std;

vector<string> split(char c, string str);
pair<string, string> split_pair(string needle, string str);
string file_get_contents(const string& path);


#endif //WEBSERV_HELPER_HPP
