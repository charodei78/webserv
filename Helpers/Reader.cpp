//
// Created by Holli Heimerdinger on 5/15/21.
//

#include "Reader.hpp"

using namespace std;

string Reader::readBefore(int fd, string const &needle, unsigned buf_size)
{
	string      result;
	string      tmp;
	long        n_pos = -1;
	char        *buf = new char[buf_size + 1]{0};
	int             error;
	int         counter = 0;

	while (true)
	{
		if (storage.empty()) {
			if ((error = read(fd, buf, buf_size)) < 0) {
				pError("read");
				delete[] buf;
				throw exception();
			}
			tmp = buf;
		}
		else
			tmp = storage;
		memset(buf, 0, buf_size);
		n_pos = tmp.find(needle);
		if (n_pos != -1)
		{
			result += tmp.substr(0, n_pos);
			storage = tmp.substr(n_pos + needle.length());
			break;
		}
		storage = "";
		result += tmp;
	}
	delete[] buf;
	return result;
}

string Reader::readLine(int fd, unsigned buf_size)
{
	return readBefore(fd, "\r\n", buf_size);
}

void Reader::clearStorage()
{
	storage = "";
}

string Reader::readCount(int fd, unsigned long count)
{
	char *buf = new char[count]{0};
	string result = storage;

	if (result.length() > count)
	{
		storage = result.substr(count + 2);
		result = result.substr(0, count);
	} else {
		read(fd, buf, count - result.length());
		result += buf;
		read(fd, buf, 2);
	}
	delete[] buf;
	return result;
}

string Reader::readFull(int fd)
{
	string  result;
	char    buf[2048] = {0};

	result = storage;
	while (read(fd, buf, 2047) > 0)
	{
		result += buf;
		memset(buf, 0, 2048);
	}
	return result;
}

Reader::Reader()
{
};

Reader::Reader(Reader const &rhs)
{
	operator=(rhs);
};

Reader::~Reader()
{
};

Reader &Reader::operator=(Reader const &rhs)
{
	if (this != &rhs) {
	}
	return *this;
};