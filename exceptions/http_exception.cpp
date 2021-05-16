//
// Created by Holli Heimerdinger on 5/7/21.
//

#include "http_exception.hpp"

Http::http_exception::http_exception(int code, string what, Config *conf )
	: code(code), message(what)
{
	this->config = new Config(*conf);
}

const char *Http::http_exception::what() const noexcept
{
	return this->message.c_str();
}

Http::http_exception::~http_exception()
{
	delete config;
}

Http::http_exception &Http::http_exception::operator=(const Http::http_exception &rhs)
{
	if (this != &rhs)
	{
		delete this->config;
		this->config = new Config(*rhs.config);
		this->code = rhs.code;
		this->message = rhs.message;
	}

	return *this;
}


