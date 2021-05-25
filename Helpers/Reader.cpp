//
// Created by Holli Heimerdinger on 5/15/21.
//

#include "Reader.hpp"

using namespace std;

int Reader::readBefore(string &result, int fd, string const &needle, unsigned buf_size)
{
	string      tmp;
	long        n_pos = -1;
	char        *buf = new char[buf_size + 1]{0};
	int         counter = 0;


	if (storage.empty() || (n_pos = storage.find(needle)) == -1) {
        if (readUsed)
            return 1;
		if ((counter = read(fd, buf, buf_size)) <= 0) {
			pError("read");
			delete[] buf;
			return -1;
		}
		readUsed = true;
		storage += buf;
		delete[] buf;
	}
	cout << "storage: " << storage << endl;

    n_pos = storage.find(needle);
	if (n_pos == -1)
		return 1;
	result = storage.substr(0, n_pos);
//	cout << "result: " << result << "storage: " << (int)storage[0] << " " << (int)storage[1 ] << endl;
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
			buf = new char[readSize]{0};
		} catch (std::bad_alloc &e) {
			buf = nullptr;
			readSize /= 2;
			if (readSize == 0)
				return -1;
		}
	}

	read_count = read(fd, buf, readSize);
	if (read_count < 0) {
		pError("read");
		delete buf;
		return -1;
	}

	if (write(fileFd, buf, read_count) != read_count)  {
		pError("write");
		delete[] buf;
		return -1;
	}

	fileSize += read_count;

	cout << "fileSize " << fileSize << endl;


	delete[] buf;

	if (count <= fileSize)
	{
		lseek(fileFd, 0, SEEK_SET);
		return 0;
	}

	return 1;
}


//int Reader::readCount(unsigned long count, int fd)
//{
//	if (count == 0)
//		return 0;
//	char *buf = new char[count]{0};
//	result = storage;
//	int read_count;
//
//	if (count > MAX_STACK_SIZE)
//		use_file = true;
//
//	if (result.length() >= count)
//	{
//		storage = result.substr(count + 2);
//		result = result.substr(0, count);
//	} else {
//		if (readUsed)
//			return 1;
//		read_count = read(fd, buf, count - result.length());
//		if (read_count < 1)
//			return -1;
//		readUsed = true;
//		if (use_file)
//		{
//			if (!fileFd)
//			{
//				fileFd = open(TMP_PATH "/tmp_big_in.txt", O_CREAT|O_RDWR|O_TRUNC, 0644);
//				if (fileFd == -1){
//					pError("open");
//					delete[] buf;
//					return -1;
//				}
//			}
//			if (write(fileFd, storage.c_str(), storage.length()) || write(fileFd, buf, read_count) != read_count) {
//				pError("write");
//				delete[] buf;
//				return -1;
//			}
//		} else {
//			result += buf;
//		}
//		delete[] buf;
//		storage = "";
//	}
//	return 0;
//}

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
};