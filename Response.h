//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H

#include <map>
#include "ENV.h"
#include <string>

using namespace std;

namespace Http {
	class Response
	{
		map<string, string> 	headers;
		string 					http_version;
		std::string 			response_body;
		unsigned 				response_status_code;
		string 					response_status_text;

	public:
		explicit Response(const string& http_version = "1.1");
		Response(Response const& rhs);
		~Response();
		Response& operator=(Response const& rhs);
		Response *code(unsigned code);
		Response *body(const string& body);
		Response *header(const string& name, const string& value);
		Response *statusText(const string& name);
		string   toString();
	};

	string get_code_message(unsigned int code);

}


#endif //WEBSERV_RESPONSE_H
