//
// Created by Holli Heimerdinger on 5/7/21.
//

#include "http_exception.hpp"

Http::http_exception::http_exception(int code, string what, Config *config )
	: code(code), message(what), config(config) {}

const char *Http::http_exception::what() const noexcept
{
	return this->message.c_str();
}
