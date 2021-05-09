//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef HELPER_HPP
#define HELPER_HPP

#include "includes.hpp"
#include "dirent.h"
#include <fstream> // TODO: #include <fstream>
#include <libc.h>

using namespace std;

vector<string> split(string needle, string str);
pair<string, string> split_pair(string needle, string str);

string file_get_contents(const string& path);

string getTimestamp();
string getIP(unsigned long ip);

string& ltrim(string& str, const string& chars = "\t\n\v\f\r ");
string& rtrim(string& str, const string& chars = "\t\n\v\f\r ");
string& trim(string& str, const string& chars = "\t\n\v\f\r ");
int isEmptySpace(int c);
bool isContain(string str, int(*predicate)(int));
int pError(string const&program);

bool is_file(string const&path);
bool is_dir(string const&path);
bool exists(string const&path);

vector<string> *get_dir_content(string const &path);

string  readLine(int fd, unsigned buf_size = 128);
string  readBefore(int fd, string const &needle, unsigned buf_size = 256);
string  readFull(int fd);
string  readCount(int fd, unsigned long count);
void    clearStorage();

int     create_dir(string path, int rights = 0777);
int file_put_contents(string filename, const string &data, int rights = 0777);

string  abs_path(string path);

template <typename T, typename  F>
bool contains(T begin, T end, F value)
{
    while (begin != end)
    {
        begin++;
        if (*begin == value)
            return true;
    }
    return false;
}

#endif //WEBSERV_HELPER_HPP
