#include "RequestParser.hpp"

RequestParser::RequestParser(const sockaddr_in addr) : addr(addr)
{
	reader = new Reader;
	bodyFd = 0;
	remains = 0;
	fileSize = 0;
	config = nullptr;
	server = nullptr;
};

RequestParser::~RequestParser()
{
	if (bodyFd)
		close(bodyFd);
	delete reader;
};

bool RequestParser::parseChunks(string &str)
{
	int length;
	size_t start;
	int totalLength = 0;
	string contentLength;

	if (remains)
		totalLength = remains;
	while (!str.empty())
	{
		try {
//			start = str.find("\r\n", totalLength + 2);
			contentLength = str.substr(totalLength, 8);
			length = stoi(contentLength, &start, 16);

//			cout << "-----" << contentLength << " " << length << endl << "-----" << endl;
			start += totalLength;
//			contentLength = str.substr(totalLength, start - totalLength);
			str.erase(totalLength, start - totalLength + 2);
//			length = stoi(contentLength, nullptr, 16);
			this->remains = length - (str.length() - totalLength);
			totalLength += length;
			if (this->remains < 0)
				this->remains = 0;
			if (length == 0) {
				str.erase(str.end() - 2, str.end());
				return true;
			}
		} catch (exception) {
//			cout << contentLength;
			return false;
		}
	}
	return false;
}

int RequestParser::parseChunked(int sock, int bodyLimit)
{
	char *buf = new char[READ_SIZE + 1]{0};
	int readRet, writeRet;
	int index;

	if (!bodyFd)
	{
		bodyFd = open(TMP_PATH "/tmp_big_in.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (bodyFd == -1){
			pError("open");
			return -1;
		}
		request.file_fd = bodyFd;
	}

	string *sBuf = new string;

	if (!reader->isEmpty()) {
		string tmp;
		reader->getStorage(tmp);

		sBuf->reserve(READ_SIZE + tmp.length() + 1);
		reader->clearStorage();
		*sBuf += tmp;
	}
	else
		sBuf->reserve(READ_SIZE + 1);

	readRet = read(sock, buf, READ_SIZE);

	*sBuf += buf;
	delete[] buf;

	bool res = parseChunks(*sBuf);

	fileSize += sBuf->length();

	writeRet = write(bodyFd, sBuf->c_str(), sBuf->length());
	delete sBuf;
	if (writeRet < 0)
		pError("write");

	if (res)
		return 0;

	if (readRet < 0)
	{
		if (errno == EAGAIN)
			return 1;
		return -1;
	}

	return 1;
}


int RequestParser::onError(int code)
{
	Server::printLog(addr, request.getLog(code));
	response.code(code);
	if (code < 499)
		response.attachDefaultHeaders(this->server->serverConfig);

	return 2;
}

int RequestParser::checkStatus(int status)
{
	if (status == -1)
		return onError(500);
	return status;
}

int RequestParser::parse(int sock, ServerListener &listener)
{
	string result;
	int status;
	int bodyLimit;

	reader->readUsed = false;
	if (!request.query.is_set) {
		status = reader->readLine(result, sock, READ_SIZE * 2);
		if (status)
			return checkStatus(status);
		request.parseQuery(result);
		if (request.query.protocol != "1.1")
			return onError(505);
	}
	// Headers
	if (request.headers.empty()) {
		status = reader->readBefore(result, sock, "\r\n\r\n", READ_SIZE);
		if (status)
			return checkStatus(status);
		request.parseHeaders(result);
		if (!this->server) {
			string host = request.headers["Host"];
			server = &listener.FindServerByHost(host);
			server = &server->GetLocationServer(request.query.address);

		}
		config = &server->serverConfig;
		bodyLimit = config->limitClientBodySize;
		if (config->auth) {
			if (!request.headers.count("Authorization"))
				return onError(401);
			string authEncoded = request.headers["Authorization"];
			authEncoded = string(find(authEncoded.begin(), authEncoded.end(), ' ') + 1, authEncoded.end());
			if (find(config->auth_file_content.begin(), config->auth_file_content.end(), authEncoded) ==
			    config->auth_file_content.end())
				return onError(401);
			//TODO: Проверить правильны ли пришедшие данные, с файла с авторизациями
		}
		if (!config->allowedFunctions.empty() && config->allowedFunctions.find(request.query.method) == -1)
			return onError(405);

		if (request.headers.count("Content-Length"))
		{
			if (bodyLimit != 0 && stoi(request.headers["Content-Length"]) > bodyLimit)
				return onError(413);
		}

	}

	if (config)
		bodyLimit = config->limitClientBodySize;


	if (request.headers["Transfer-Encoding"] == "chunked") {
		int ret = parseChunked(sock, bodyLimit);
		if (bodyLimit && fileSize > bodyLimit)
			return onError(413);
		if (ret != 0)
			return ret;
		lseek(bodyFd, 0, SEEK_SET);
	} else {
		if (request.headers.count("Content-Length")) {
			int size = stoi(request.headers["Content-Length"]);

			if (bodyLimit != 0 && size > bodyLimit)
				return onError(413);

			status = reader->readCount(size, sock);
			if (status)
				return checkStatus(status);

			if (!reader->use_file)
				request.body += result;
			else
				request.file_fd = reader->fileFd;
		} else {
			status = reader->getStorage(result);
			request.body = result;
			if (status)
				return checkStatus(status);
		}
	}

	try {
		response = server->getHttpResponse(addr, sock, &request, config);
		if (response.code() >= 200 && response.code() < 300) {
			if (!response.body().empty() && request.headers["Transfer-Encoding"] == "chunked")
				response.header("Transfer-Encoding", "chunked");
			Server::printLog(addr, request.getLog(response.code()));
			return 2;
		}
		return onError(response.code());
	} catch (exception &e) {
		response.code(500);
		try {
			response.putFile(DEFAULT_ERROR_PAGE);
		} catch (exception &e) {
			cerr << e.what() << endl;
		}
		return onError(500);
	}

	return 0;
}
