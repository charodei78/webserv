//
// Created by Harren Leilani on 5/23/21.
//

#include "Client.hpp"

Client::Client(int sock, sockaddr_in addr)
{
    currentState = requestParsing;
    reader = new Reader();
    this->sock = sock;
	this->config = nullptr;
    this->server = nullptr;
    this->addr = addr;
    time(&lastOperationTime);
}

int Client::getSock() {
    return sock;
}


int Client::sendResponse() {
    time(&lastOperationTime);
    if (responseBuffer.empty())
        responseBuffer = response.toString();
    int sended = send(sock, responseBuffer.c_str(), responseBuffer.length(), 0);
    if (sended <= 0)
        return sended;
    responseBuffer = responseBuffer.erase(responseBuffer.size() - sended);
    if (responseBuffer.empty())
        currentState = closeConnection;
    return 1;
}

int Client::checkStatus(int status)
{
	if (status == -1)
		return onError(500);
	return status;
}

int Client::onError(int code)
{
    Server::printLog(addr, request.getLog(code));
    response.code(code);
    if (code != 505)
        response.attachDefaultHeaders(this->server->serverConfig);

    currentState = sendingResponse;
    return 1;
}
int Client::readRequest(ServerListener &listener)
{
    string result;
    int status;
    int read_count;
    int BodyLimit;
    // Querry

    reader->reedUsed = false;
    time(&lastOperationTime);
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
        // TODO: max size 413
        request.parseHeaders(result);
        if (!this->server) {
            string host = request.headers["Host"];
            server = &listener.FindServerByHost(host);
            server = &server->GetLocationServer(request.query.address);
        }
        config = &server->serverConfig;
        BodyLimit = config->limitClientBodySize;
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
            if (BodyLimit != 0 &&  stoi(request.headers["Content-Length"]) > BodyLimit)
                return onError(405);

    }
    // Body
    if (request.headers["Transfer-Encoding"] == "chunked") {
        status = reader->readLine(result, sock, READ_SIZE);
        if (status)
            return checkStatus(status);
        read_count = stoi(result, nullptr, 16);
        status = reader->readCount(result, read_count, sock);
        if (status)
            return checkStatus(status);
        if (!reader->use_file) {
	        request.body += result;
	        if (BodyLimit != 0 && request.body.length() > BodyLimit)
		        return onError(413);
        } else
        	request.file_fd = reader->file_fd;
    } else {
        if (request.headers.count("Content-Length")) {
        	int size = stoi(request.headers["Content-Length"]);

        	if (BodyLimit != 0 && size > BodyLimit)
		        return onError(413);

	        status = reader->readCount(result, size, sock);
            if (status)
                return checkStatus(status);

	        if (!reader->use_file)
		        request.body += result;
	        else
		        request.file_fd = reader->file_fd;
        } else {
            status = reader->getStorage(result);
	        request.body = result;
            if (status)
                return checkStatus(status);
        }
    }

    try {
        response = server->SendHttpResponse(addr, sock, &request, config);
        if (response.code() >= 200 && response.code() < 300) {
            currentState = sendingResponse;
	        Server::printLog(addr, request.getLog(response.code()));
	        return 1;
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

Client &Client::operator=(const Client &src) {
    if (src.sock == 0)
        return *this;
    if (this != &src) {
        this->lastOperationTime = src.lastOperationTime;
        this->sock = src.sock;
        this->reader = src.reader;
        this->request = src.request;
        this->response = src.response;
        this->currentState = src.currentState;
        this->readBuffer = src.readBuffer;
        this->sendBuffer = src.sendBuffer;
        this->attachedServer = src.attachedServer;
        this->addr = src.addr;
    }
    return *this;
}

Client::Client(const Client &c) {
    *this = c;
}

Client::~Client() {
}
