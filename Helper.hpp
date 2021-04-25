//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef WEBSERV_HELPER_HPP
#define WEBSERV_HELPER_HPP

#include "includes.hpp"
#include <fstream> // TODO: #include <fstream>
#include <libc.h>

using namespace std;

vector<string> split(char c, string str);
pair<string, string> split_pair(string needle, string str);

string file_get_contents(const string& path);

string getTimestamp();
string getIP(unsigned long ip);

string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

string  abs_path(string path);


#endif //WEBSERV_HELPER_HPP
