//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes.hpp"

using namespace std;

namespace Http {

	class Request;

	class Response
	{
		map<string, string> 	headers;
		std::string 			response_body;
		unsigned 				response_status_code;
		string 					response_status_text;

	public:
		bool isCgi;

		explicit Response();
		Response(Response const& rhs);
		~Response();
		Response&       operator=(Response const& rhs);
		Response&       operator=(string const& rhs);
		Response        *code(unsigned code);
		unsigned        code();
		Response        *body(const string& body);
		const string&   body();
		Response        *header(const string& name, const string& value);
		string          header(const string& name);
		Response        *statusText(const string& name);
		string toString();

		Response *putFile(const string &path);

		Response *attachDefaultHeaders(Config const &config);


	};

	string get_code_message(unsigned int code);

}


#endif //WEBSERV_RESPONSE_HPP
