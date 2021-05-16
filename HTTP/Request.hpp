//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../includes.hpp"

using namespace std;

namespace Http {

	struct query_s {
		string method;
		string address;
		string query_string;
		string protocol;
	};

	class Request
	{
	public:
		query_s query;
		string  body;
		map<string, string> headers;
		int     from;

		Request();
		Request *parseQuery(const string & str);
		Request *parseHeaders(const string & str);

		Request(const string& body);
		Request(Request const &rhs);
		~Request();
		Request &operator=(Request const &rhs);

		string getLog(unsigned int code = 200);
	};

}

#endif //WEBSERV_REQUEST_HPP
