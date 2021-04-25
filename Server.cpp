#include "Server.hpp"

void Server::printLog(sockaddr_in client_addr, const string &message = "")
{
	cout << "[" << getTimestamp() << "] "
	     << getIP(client_addr.sin_addr.s_addr) << ":" << client_addr.sin_port << " "
	     << message << endl;
}

Server::Server(const Config config) : serverConfig(config)
{
}

Server::Server(Server const &rhs) : serverConfig(rhs.serverConfig)
{
}

Server &Server::operator=(Server const &rhs)
{
	return *this;
}

std::string Server::GetServerName()
{
	return serverName;
}

bool Server::operator==(Server server)
{
	return serverName == server.serverName;
}

Server::~Server()
{
	if (socket_fd > 0)
		close(socket_fd);
}

bool Server::Intialize()
{
	sockaddr_in addr = sockaddr_in();
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(serverConfig.port);
	addr.sin_family = AF_INET;
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	//Проверка удалось ли инициализировать сокет
	if (socket_fd < 0 || bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		std::cerr << strerror(errno);
		return false;
	}
	return true;
}


//void Server::closeServer();

void Server::StartListening()
{
	sockaddr_in client_addr;

	if (listen(socket_fd, serverConfig.clientLimit) < 0)
		return;

	cout << "Starting server: http://" << serverConfig.domain << ":" << serverConfig.port << endl;

	int addrlen = sizeof(client_addr);

	for (;;) {
		int client_socket = accept(socket_fd, (struct sockaddr *) &client_addr,
		                           reinterpret_cast<socklen_t *>(&addrlen));
		if (client_socket == 0)
			continue;
		else {
			if (client_socket < 0) {
				cerr << strerror(errno);
				return;
			}
		}
		printLog(client_addr, "Accepted");
		ProcessConnection(client_addr, client_socket);

		close(client_socket);
		printLog(client_addr, "Closed");
	}
}

void Server::ProcessConnection(const sockaddr_in &addr, const int sock)
{
	fd_set rfds;
	timeval limitTime;
	limitTime.tv_sec = 2;
	limitTime.tv_usec = 0;
	int result;
	string message;
	Http::Request *request;
	Http::Response *response;

	FD_ZERO(&rfds);

	FD_SET(sock, &rfds);

	int retval = select(sock + 1, &rfds, NULL, NULL, &limitTime);

	if (retval && FD_ISSET(sock, &rfds)) 
	{
		char buf[256];
		bzero(buf, 256);
		std::string requestString = "";
		int count;
		while ((count = read(sock, buf, 255)) > 0 && count < 256) 
		{
			requestString += buf;
			if (count != 255)
				break;
			bzero(buf, 256);
		}
		SendHttpResponse(addr, sock, requestString);
	}
	else
		return;
}

bool Server::SendHttpResponse(const sockaddr_in &addr, const int sock, std::string requestString)
{
	int result;
	std::string message;
	Http::Request *request;
	Http::Response *response;
	CGIRequest  *cgiRequest;

	try 
	{
		request = new Http::Request(requestString);
		response = new Http::Response();
		if (request->query.address == "/") 
		{
			response
					->body(file_get_contents(this->serverConfig.root_directory + "/image.html"))
					->header("Content-Type", "text/html");
		}
		else if (request->query.address.find("php") != string::npos) {
			cgiRequest = new CGIRequest(*request, serverConfig, addr);
			cgiRequest->makeQuery();
		} else {
			response->putFile(this->serverConfig.root_directory + request->query.address);
		}
		result = send(sock, response->toString().data(), response->toString().length(), 0);
		if (result == -1) {
			// sending failed
			cerr << "send failed" << endl;
		}
		message = "[" + to_string(response->code()) + "]: "
						+ request->query.method + " " + request->query.address;
		if (!request->query.query_string.empty())
			message += "?" + request->query.query_string;
		printLog(addr,  message);

		delete response;

	} catch (exception e) {
		// query string is incorrect
		printLog(addr,  strerror(errno));
		return false;
	}
	return true;
}