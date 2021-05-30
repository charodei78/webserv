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
public:
	Reader();
	~Reader();
	bool    readUsed;
	bool    use_file;
	int     fileFd;
	size_t  fileSize;


	Reader(Reader const &rhs);
	Reader &operator=(Reader const &rhs);

	int     readLine(string &result, int fd, unsigned buf_size = 128);
	int     readBefore(string &result, int fd, string const &needle, unsigned buf_size = 256);
//	int     readOne(string &result, int fd);
	bool    isEmpty();
	int readCount(unsigned long count, int fd);
	void    clearStorage();

    int getStorage(string &result);
};


#endif //WEBSERV_READER_HPP
