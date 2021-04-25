//
// Created by Holli Heimerdinger on 4/20/21.
//

#include "Request.hpp"

using namespace Http;

query_s parse_query(const string &str)
{
	query_s             res;
	vector<string>      parts = split(' ', str);
	pair<string,string> query;

	if (parts.size() != 3 || parts[0].empty() || parts[1].empty() || parts[2].empty())
		throw exception();
	res.method = trim(parts[0]);
	query = split_pair("?" ,trim(parts[1]));
	res.address = query.first;
	res.query_string = query.second;
	res.protocol = parts[2].substr(5);
	res.protocol = trim(res.protocol);
	return res;
}

Request::Request(const string &request)
{
	pair<string, string> message = split_pair("\n\r\n", request);
	if (message.second.empty())
		message = split_pair("\n\n", request);
	this->body = message.second;

	vector<string> structure = split('\n', message.first);

	if (structure.size() < 2)
		throw exception();
	try {
		this->query = parse_query(structure[0]);
	} catch (exception e) {
		// TODO: catch query string
	}

	for (int i = 1; i < structure.size(); ++i) {
		this->headers.insert(split_pair(": ", structure[i])); // TODO: прочитать стандарт насчет пробела
	}
};

Request::Request(Request const &rhs)
{
	operator=(rhs);
};

Request::~Request()
{
};

Request &Request::operator=(Request const &rhs)
{
	if (this != &rhs) {
	}
	return *this;
};