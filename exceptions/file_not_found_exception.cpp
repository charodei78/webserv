//
// Created by Holli Heimerdinger on 5/16/21.
//

#include "file_not_found_exception.hpp"

file_not_found_exception::file_not_found_exception()
{
};

file_not_found_exception::~file_not_found_exception()
{
};


const char *file_not_found_exception::what() const noexcept
{
	return "File not found";
};