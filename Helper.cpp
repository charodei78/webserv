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


