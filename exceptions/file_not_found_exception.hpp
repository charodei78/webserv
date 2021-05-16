//
// Created by Holli Heimerdinger on 5/16/21.
//

#ifndef WEBSERV_FILE_NOT_FOUND_EXCEPTION_HPP
#define WEBSERV_FILE_NOT_FOUND_EXCEPTION_HPP

#include "exception"
#include "string"

using namespace std;

class file_not_found_exception: public exception
{

public:
	file_not_found_exception();
	~file_not_found_exception();

	const char *what() const _NOEXCEPT override;

};


#endif //WEBSERV_FILE_NOT_FOUND_EXCEPTION_HPP
