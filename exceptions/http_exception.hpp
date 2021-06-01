//
// Created by Holli Heimerdinger on 5/7/21.
//

#ifndef WEBSERV_HTTP_EXCEPTION_HPP
#define WEBSERV_HTTP_EXCEPTION_HPP

#include <exception>
#include <string>
#include "../Server/Config/Config.hpp"

using namespace std;

namespace Http {
	class http_exception: public std::exception
	{
		public:
			int code;
			string message;
			Config config;

			http_exception(int code, string what = "", Config *config = nullptr);
			http_exception &operator=(http_exception const& rhs);
			~http_exception() throw() {}
			const char * what() const throw();
	};
};


#endif //WEBSERV_HTTP_EXCEPTION_HPP
