//
// Created by Holli Heimerdinger on 5/7/21.
//

#ifndef WEBSERV_HTTP_EXCEPTION_HPP
#define WEBSERV_HTTP_EXCEPTION_HPP

#include <exception>
#include <string>
#include "Config.hpp"

using namespace std;

namespace Http {
	class http_exception: public std::exception
	{
	public:
		int code;
		string message;
		Config *config;

		http_exception(int code, string what = "", Config *config = nullptr);
		const char * what() const noexcept override;
	};
};


#endif //WEBSERV_HTTP_EXCEPTION_HPP
