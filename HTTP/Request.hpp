//
// Created by Holli Heimerdinger on 4/20/21.
//

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../includes.hpp"

using namespace std;

namespace Http {

    struct query_s {
        string  method;
        string  address;
        string  query_string;
        string  protocol;
        bool    is_set;
        query_s():method(""),
                  address(""),
                  query_string(""),
                  protocol(""),
                  is_set(false)
        {};
    };

	class Request
	{
	public:
		query_s query;
		string  body;
		int     file_fd;
		map<string, string> headers;
		int     from;

		Request();
		Request *parseQuery(const string & str);
		Request *parseHeaders(const string & str);

		Request(const string& body);
		Request(Request const &rhs);
		~Request();
		Request &operator=(Request const &rhs);

		int parseChunkedBody(int fd);
		string getLog(unsigned int code = 200);
	};

}

#endif //WEBSERV_REQUEST_HPP
