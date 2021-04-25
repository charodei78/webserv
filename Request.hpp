//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "includes.hpp"

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
		Request();
	public:
		query_s query;
		string  body;
		map<string, string> headers;
		int     from;

		Request(const string& body);
		Request(Request const &rhs);
		~Request();
		Request &operator=(Request const &rhs);
	};

}

#endif //WEBSERV_REQUEST_HPP
