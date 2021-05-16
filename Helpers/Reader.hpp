//
// Created by Holli Heimerdinger on 5/15/21.
//

#ifndef WEBSERV_READER_HPP
#define WEBSERV_READER_HPP

#include <string>
#include <unistd.h>
#include "Helper.hpp"


class Reader
{
	string storage;
	Reader(Reader const &rhs);
	Reader &operator=(Reader const &rhs);
public:
	Reader();
	~Reader();

	string  readLine(int fd, unsigned buf_size = 128);
	string  readBefore(int fd, string const &needle, unsigned buf_size = 256);
	string  readFull(int fd);
	string  readCount(int fd, unsigned long count);
	void    clearStorage();

};


#endif //WEBSERV_READER_HPP
