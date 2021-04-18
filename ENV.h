//
// Created by Holli Heimerdinger on 4/18/21.
//

#ifndef WEBSERV_ENV_H
#define WEBSERV_ENV_H

#include <string>

namespace Http {
	static int codes[54] = { 100,101,103,200,201,202,203,204,205,206,300,301,302,303,304,307,308,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,422,425,426,428,429,431,451,500,501,502,503,504,505,506,507,508,510,511};
	static std::string messages[54] = {
		"Continue",
		"Switching Protocols",
		"Early Hints",
		"OK",
		"Created",
		"Accepted",
		"Non-Authoritative Information",
		"No Content",
		"Reset Content",
		"Partial Content",
		"Multiple Choices",
		"Moved Permanently",
		"Found",
		"See Other",
		"Not Modified",
		"Temporary Redirect",
		"Permanent Redirect",
		"Bad Request",
		"Unauthorized",
		"Payment Required",
		"Forbidden",
		"Not Found",
		"Method Not Allowed",
		"Not Acceptable",
		"Proxy Authentication Required",
		"Request Timeout",
		"Conflict",
		"Gone",
		"Length Required",
		"Precondition Failed",
		"Payload Too Large",
		"URI Too Long",
		"Unsupported Media Type",
		"Range Not Satisfiable",
		"Expectation Failed",
		"I'm a teapot",
		"Unprocessable Entity",
		"Too Early",
		"Upgrade Required",
		"Precondition Required",
		"Too Many Requests",
		"Request Header Fields Too Large",
		"Unavailable For Legal Reasons",
		"Internal Server Error",
		"Not Implemented",
		"Bad Gateway",
		"Service Unavailable",
		"Gateway Timeout",
		"HTTP Version Not Supported",
		"Variant Also Negotiates",
		"Insufficient Storage",
		"Loop Detected",
		"Not Extended",
		"Network Authentication Required"
	};
};


#endif //WEBSERV_ENV_H
