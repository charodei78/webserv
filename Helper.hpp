//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef HELPER_HPP
#define HELPER_HPP

#include "includes.hpp"
#include <fstream> // TODO: #include <fstream>
#include <libc.h>

using namespace std;

vector<string> split(char c, string str);
pair<string, string> split_pair(string needle, string str);

string file_get_contents(const string& path);

string getTimestamp();
string getIP(unsigned long ip);

string& ltrim(string& str, const string& chars = "\t\n\v\f\r ");
string& rtrim(string& str, const string& chars = "\t\n\v\f\r ");
string& trim(string& str, const string& chars = "\t\n\v\f\r ");
int isEmptySpace(int c);
bool isContain(string str, int(*predicate)(int));

string  abs_path(string path);


#endif //WEBSERV_HELPER_HPP
