//
// Created by Holli Heimerdinger on 4/20/21.
//

#include "Request.hpp"

using namespace Http;


Request::Request(const string &request)
{
	size_t pos;
	string query_string;
	string headers_string;

	pair<string, string> message = split_pair("\n\r\n", request);
	if (message.second.empty())
		message = split_pair("\n\n", request);
	pos = message.first.find("/n");
	if (pos == string::npos)
		throw exception();

	query_string = message.first.substr(0, pos - 1);
	parseQuery(query_string);

	headers_string = message.first.substr(pos + 1);
	parseHeaders(headers_string);
	body = message.second;
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
		query = rhs.query;
		body = rhs.body;
		headers = rhs.headers;
		from = rhs.from;
	}
	return *this;
}

Request *Request::parseQuery(const string &str)
{
	vector<string>      parts = split(' ', str);
	pair<string,string> args;

	if (parts.size() != 3 || parts[0].empty() || parts[1].empty() || parts[2].empty())
		throw exception();
	query.method = trim(parts[0]);
	args = split_pair("?" , trim(parts[1]));
	query.address = args.first;
	query.query_string = args.second;
	query.protocol = parts[2].substr(5);
	query.protocol = trim(query.protocol);
	return this;
}

Request *Request::parseHeaders(const string &str)
{
	vector<string> structure = split('\n', str);

	for (int i = 1; i < structure.size(); ++i) {
		this->headers.insert(split_pair(": ", structure[i])); // TODO: прочитать стандарт насчет пробела
	}
	return nullptr;
}

Request::Request()
{

}

