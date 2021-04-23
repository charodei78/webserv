//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "includes.hpp"

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
		Response&       operator=(Response const& rhs);
		Response        *code(unsigned code);
		unsigned        code();
		Response        *body(const string& body);
		const string&   body();
		Response        *header(const string& name, const string& value);
		const string&   header(const string& name);
		Response        *statusText(const string& name);
		string          toString();

		Response *putFile(const string &path);
	};

	string get_code_message(unsigned int code);

}


#endif //WEBSERV_RESPONSE_HPP
