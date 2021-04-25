//
// Created by Holli Heimerdinger on 4/20/21.
//

#include "Helper.hpp"

vector<string> split(char c, string str) {
	size_t len = str.length();
	vector<string> result;
	size_t start = 0;
	size_t end = 0;

	while (start < len)
	{
		end = str.find(c, start);
		if (end == string::npos)
			end = len;
		result.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	return result;
};

pair<string, string> split_pair(string needle, string str) {
	size_t index;
	string header;
	string body;
	size_t len = str.length();

	index = str.find(needle, 0);
	if (index == string::npos) {
		return make_pair(str, "");
	}
	header = str.substr(0, index);
	body = str.substr(index + needle.length(), len - index);
	return make_pair(header, body);
}

string file_get_contents(const string& path)
{
	std::ifstream t(path);
	if (!t)
		throw exception();
	std::string str((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
	return str;
}

string getTimestamp()
{
	char        buffer[256] = "";
	tm          *time;
	timeval     timeval = {};

	gettimeofday(&timeval, nullptr);
	time = localtime(&timeval.tv_sec);
	mktime(time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", time);
	return string(buffer);
}

string getIP(unsigned long ip)
{
	string result = "";

//	cout << (ip & 0xFF000000);
	result += to_string(ip & 0xFF) + '.';
	result += to_string(ip >> 8 & 0xFF) + '.';
	result += to_string(ip >> 16 & 0xFF) + '.';
	result += to_string(ip >> 24 & 0xFF);
	return result;
}

std::string& ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}


string  abs_path(string path) {
	string  result;
	char    buf[512] = {};

	if (path[0] == '/')
		return path;
	getcwd(buf, sizeof(buf));
	result = buf;

	while (1) {
		if (path.find("../") == 0) {
			result = result.substr(0, result.find_last_of('/'));
			path = path.substr(3);
		}
		else if (path.find("./") == 0) {
			path = path.substr(2);
			continue ;
		}
		else
		{
			if (path[0] == '/')
				return result + path;
			else
				return result += "/" + path;
		}
	}
}
