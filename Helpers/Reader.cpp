//
// Created by Holli Heimerdinger on 5/15/21.
//

#include "Reader.hpp"

using namespace std;

int Reader::readBefore(string &result, int fd, string const &needle, unsigned buf_size)
{
	string      tmp;
	long        n_pos;
	char        *buf;

	if (storage.empty() || (n_pos = storage.find(needle)) == -1) {
        if (readUsed)
            return 1;
		buf = new char[buf_size + 1];
		bzero(buf, buf_size + 1);

		if (read(fd, buf, buf_size) <= 0) {
			if (errno != ECONNRESET && errno != ETIMEDOUT)
				pError("read");
			delete[] buf;
			return -1;
		}
		readUsed = true;
		storage += buf;
		delete[] buf;
	}

    n_pos = storage.find(needle);
	if (n_pos == -1)
		return 1;
	result = storage.substr(0, n_pos);
    storage = storage.substr(n_pos + needle.length());
	return 0;
}

int Reader::readLine(string &result, int fd, unsigned buf_size)
{
	return readBefore(result,fd, "\r\n", buf_size);
}

void Reader::clearStorage()
{
	storage = "";
}

int Reader::readCount(unsigned long count, int fd)
{

	if (count == 0)
		return 0;

	use_file = true;

	if (!fileFd)
	{
		fileSize = 0;
		fileFd = open(TMP_PATH "/tmp_big_in.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fileFd == -1){
			pError("open");
			return -1;
		}
	}
	if (storage.length()) {
		if (write(fileFd, storage.c_str(), storage.length()) < 0) {
			pError("write");
			storage = "";
			return -1;
		}
		fileSize += storage.length();
		storage = "";
	}

	char *buf = nullptr;
	size_t read_count;

	cout << count << "  " << fileSize << endl;

	int readSize = count - fileSize;

	if (count <= fileSize)
	{
		lseek(fileFd, 0, SEEK_SET);
		return 0;
	}

	if (readUsed)
		return 1;
	readUsed = true;

	while (!buf) {
		try {
			buf = new char[readSize];
			bzero(buf, readSize);
		} catch (std::bad_alloc &e) {
			buf = nullptr;
			readSize /= 2;
			if (readSize == 0)
				return -1;
		}
	}

	read_count = read(fd, buf, readSize);
	if (read_count < 0) {
		if (errno != ECONNRESET && errno != ETIMEDOUT)
			pError("read");
		delete buf;
		return -1;
	}

	if (write(fileFd, buf, read_count) != (long)read_count)  {
		pError("write");
		delete[] buf;
		return -1;
	}

	fileSize += read_count;

	delete[] buf;

	if (count <= fileSize)
	{
		lseek(fileFd, 0, SEEK_SET);
		return 0;
	}

	return 1;
}

Reader::Reader(): readUsed(false)
{
	use_file = false;
	fileFd = 0;
	fileSize = 0;
    storage = "";
};

Reader::Reader(Reader const &rhs)
{
	operator=(rhs);
};

Reader::~Reader()
{
	if (fileFd)
		close(fileFd);
};

Reader &Reader::operator=(Reader const &rhs)
{
	if (this != &rhs) {
		this->storage = rhs.storage;
	}
	return *this;
}

int Reader::getStorage(string &result) {
    result = storage;
    return 0;
}

bool Reader::isEmpty()
{
	return storage.empty();
};