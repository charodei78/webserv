//
// Created by Holli Heimerdinger on 4/20/21.
//

#include "Helper.hpp"
#include "../exceptions/file_not_found_exception.hpp"

vector<string> split(string needle, string str)
{
	size_t len = str.length();
	vector<string> result;
	size_t start = 0;
	size_t end = 0;

	while (start < len)
	{
		end = str.find(needle, start);
		if (end == string::npos)
			end = len;
		result.push_back(str.substr(start, end - start));
		start = end + needle.length();
	}
	return result;
};

bool isContain(string str, int(*predicate)(int))
{
    return find_if(str.begin(), str.end(), predicate) != str.end();
}

int isEmptySpace(int c) 
{ 
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

pair<string, string> split_pair(string needle, string str) {
	size_t index;
	string header;
	string body;
	size_t len = str.length();

	index = str.find(needle, 0);
	if (index == string::npos) {
		return make_pair(str, "");
	}
	header = str.substr(0, index);
	body = str.substr(index + needle.length(), len - index);
	return make_pair(header, body);
}

string file_get_contents(const string& path)
{
    std::ifstream t(path);
    if (!t)
        throw file_not_found_exception();
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

int     file_put_contents(string filename, int fd, int rights)
{
	int        index;
	int             target;
	int             count;
	unsigned char   buf[10240] = {0};

	if (filename.empty())
		return -1;
	filename = abs_path(filename);
	if (!is_file(filename)) {
		if (is_dir(filename))
			return -1;
		index = filename.find_last_of('/');
		if (index != -1)
			if (create_dir(filename.substr(0, index)) == -1)
				return pError("mkdir");
	}
	target = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, rights);
	if (target == -1)
		return pError("open");

	do {
		count = read(fd, buf, 10239);
		if (count == -1)
			unlink(filename.c_str());
		if (!count)
			continue;
		if (write(target, buf, count) == -1)
		{
			pError("write");
			if (unlink(filename.c_str()) < 0)
				pError("unlink");

			close(target);
			return -1;
		}
		memset(buf, 0, 10240);
	} while (count != 0);
	close(target);
	return 0;
}


int create_dir(string path, int rights)
{
	int    index;
	int         result;
	string      target;

	result = 0;
	if (path[0] != '/')
		path = abs_path(path);
	if (is_dir(path))
		return (0);
	if (path == ".." || path == ".")
		return (0);
	index = path.find_last_of('/');
	if (index != -1) {
		target = path.substr(0, index);
		if (!is_dir(target))
			result = create_dir(target, rights);
	}
	if (result == -1)
		return -1;
	return mkdir(path.c_str(), rights);
}

int file_put_contents(string filename, const string &data, int rights)
{
	int    index;
	int         fd;

	if (filename.empty())
		return -1;
	filename = abs_path(filename);
	if (!is_file(filename)) {
		if (is_dir(filename))
			return -1;
		index = filename.find_last_of('/');
		if (index != -1)
			if (create_dir(filename.substr(0, index)) == -1)
				return pError("mkdir");
	}
	fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, rights);
	if (fd == -1)
		return pError("open");
	if (write(fd, data.c_str(), data.size()) == -1)
	{
		pError("write");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}


string to_http_date(time_t const &timeval)
{
	char        buffer[256] = "";
	tm          *time;

	time = localtime(&timeval);
	mktime(time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", time);
	return buffer;
}

string get_http_timestamp()
{
	timeval     timeval = {};

	gettimeofday(&timeval, nullptr);

	return to_http_date(timeval.tv_sec);
}

string getIP(unsigned long ip)
{
	string result = "";

//	cout << (ip & 0xFF000000);
	result += to_string(ip & 0xFF) + '.';
	result += to_string(ip >> 8 & 0xFF) + '.';
	result += to_string(ip >> 16 & 0xFF) + '.';
	result += to_string(ip >> 24 & 0xFF);
	return result;
}

string last_modified(string path)
{
	struct stat buf;
	bzero(&buf, sizeof(struct stat));

	if (stat(path.c_str(), &buf) == -1)
		return "";
	return to_http_date(buf.st_ctimespec.tv_sec);
}

int IsEmptySpace(int c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

std::string& ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}

string  abs_path(string path) {
	string  result;
	string  tmp;
	char    buf[512] = {};
	vector<string> folders;

	if (path[0] == '/')
		return path;
	getcwd(buf, sizeof(buf));
	result = buf;
	folders = split("/", path);

	for (unsigned i = 0; i < folders.size(); i++) {
		tmp = folders[i];
		if (tmp == "..")
			result.erase(result.find_last_of('/'), result.size());
		else if (tmp == "." || tmp.empty())
			continue ;
		else
			result += "/" + tmp;
	}
	return result;
}

int pError(string const&program)
{
	std::cerr << program << ": " << strerror(errno) << std::endl;
	return -1;
}

bool is_file(string const&path)
{
	struct stat buf;
	bzero(&buf, sizeof(struct stat));
	if (stat(path.c_str(), &buf) == -1)
		return false;
	return S_ISREG(buf.st_mode);
}

bool is_dir(string const&path)
{
	struct stat buf;
	bzero(&buf, sizeof(struct stat));
	if (stat(path.c_str(), &buf) == -1)
		return false;
	return S_ISDIR(buf.st_mode);
}

bool exists(string const&path)
{
	struct stat buf;
	bzero(&buf, sizeof(struct stat));
	if (stat(path.c_str(), &buf) == -1)
		return false;
	return true;
}

vector<string> *get_dir_content(string const &path)
{
	vector<string> *result;
	DIR *dir = opendir(path.c_str());
	struct dirent* dirent;

	if (!dir)
		return nullptr;

	result = new vector<string>;

	while ((dirent = readdir(dir)) != nullptr)
		result->push_back(dirent->d_name);

	closedir(dir);

	return result;
}

string toChunked(string body)
{
	int i = 0;
	int op_size = OPERATION_BYTE_SIZE;
	int remains;
	string stringSize;


	for (;;) {
		cout << i << endl;
		if (i != 0)
		{
			body.insert(i, "\r\n");
			i += 2;
		}
		if ((int)body.length() - i <= op_size)
		{
			remains = body.length() - i;
			body.insert(i, itoa(remains, 16) + "\r\n");
			body += "\r\n0\r\n\r\n";
			break;
		} else {
			stringSize = itoa(op_size, 16) + "\r\n";
			body.insert(i,   stringSize);
			i += stringSize.length() + op_size;
		}
	}


	return body;
}

int		match(const char *s1, const char *s2)
{
	if (*s1 != '\0' && *s2 == '*')
		return (match(s1 + 1, s2) || match(s1, s2 + 1));
	if (*s1 == '\0' && *s2 == '*')
		return (match(s1, s2 + 1));
	if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
		return (match(s1 + 1, s2 + 1));
	if (*s1 == '\0' && *s2 == '\0')
		return (1);
	return (0);
}

int		n_match(const char *s1, const char *s2)
{
	if (*s1 != '\0' && *s2 == '*')
		return (max<int>(match(s1 + 1, s2), match(s1, s2 + 1)));
	if (*s1 == '\0' && *s2 == '*')
		return (match(s1, s2 + 1));
	if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
		return (1 + match(s1 + 1, s2 + 1));
	if (*s1 == '\0' && *s2 == '\0')
		return (1);
	return (0);
}

int cmp_word(string needle, string haystack, int limit)
{
	int distances[3];

	if (limit < 0)
		return 1;

	if (limit < 0 || (needle == "" && haystack == "")) return 0;
	if (needle.length() == 0 && haystack.length() != 0) return 1 + cmp_word(needle, haystack.substr(1), limit - 1);
	if (needle.length() != 0 && haystack.length() == 0) return 1 + cmp_word(needle.substr(1), haystack, limit - 1);
	if (needle[0] == haystack[0]) return cmp_word(needle.substr(1), haystack.substr(1), limit);


	// try change
	limit -= 1;
	distances[0] = 1 + cmp_word(needle.substr(1), haystack.substr(1), limit);
	if (distances[0] == 0)
		return 0;
	distances[1] = 1 + cmp_word(needle, haystack.substr(1), limit);
	if (distances[1] == 0)
		return 0;
	distances[2] = 1 + cmp_word(needle.substr(1), haystack, limit);
	if (distances[2] == 0)
		return 0;
	return min<int>(min<int>(distances[0], distances[1]), distances[2]);
}

// unsafe max value;

char      num_to_hex(int value)
{
	if (value > 15)
		return 0;
	if (value <= 9)
		return '0' + value;
	else
		return 'A' + value - 10;
}

string    itoa(int value, int base)
{
	string sign;

	if (base < 0) {
		sign = '-';
		value *= -1;
	}
	if (base < 2 )
		return "";
	if (value >= base)
		return sign + itoa(value / base, base) + num_to_hex(value % base);
	else
		return sign + num_to_hex(value % base);
}

string      toUpper(string str)
{
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str;
}

string      toCgiHeader(string str)
{
	string result = "HTTP_";

	replace(str.begin(), str.end(), '-', '_');
	str = toUpper(str);
	return result + str;
}