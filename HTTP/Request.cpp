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
		throw Http::http_exception(400, "400");

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
	vector<string>      parts = split(" ", str);
	pair<string,string> args;

	if (parts[0].empty() || parts[1].empty())
		throw Http::http_exception(400, "400");
    query.is_set = true;
	query.method = trim(parts[0]);
	args = split_pair("?" , trim(parts[1]));
	query.address = args.first;
	query.query_string = args.second;
	if (parts.size() == 3) {
        query.protocol = parts[2].substr(5);

	}
	else
	    query.protocol = "1.1";
	query.protocol = trim(query.protocol);
	return this;
}

Request *Request::parseHeaders(const string &str)
{
	vector<string> structure = split("\r\n", str);

	for (unsigned i = 0; i < structure.size(); ++i) {
		this->headers.insert(split_pair(": ", structure[i])); // TODO: прочитать стандарт насчет пробела
	}
	return nullptr;
}

Request::Request()
{
	file_fd = 0;
}

string Request::getLog(unsigned int code)
{
	string message = "[" + to_string(code) + "]: "
	                 + query.method + " " + query.address;
	if (!query.query_string.empty())
		message += "?" + query.query_string;
	return message;
}

int Request::parseChunkedBody(int fd)
{
//	int size = 0;
//	string *result = new string[body.size()]{0};
//	int index;
//
//	do {
//		index = body.find("\r\n");
//		size = stoi(body.substr(0, index), 0,16);
//		*result += body.substr(index, size);
//		body = body.substr(size + 2);
//	} while (size != 0);
	return fd * 0;
}
