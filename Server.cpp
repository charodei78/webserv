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

//_Noreturn void Server::handle()
//{
//	int         sock_c;
//	sockaddr_in addr_c;
//	int         addrlen;
//	int         result;
//	string      requestString;
//	Http::Request     *request;
//	Http::Response    *response;
//	unsigned    count;
//	char        buf[256];
//
//	while (1)
//	{
//		addrlen = sizeof addr_c;
//		if ((sock_c = accept(this->socket_fd, (struct sockaddr*)&addr_c, reinterpret_cast<socklen_t *>(&addrlen))) != 0)
//		{
//			bzero(buf, 256);
//			requestString = "";
//			while ((count = read(sock_c, buf, 255)) > 0 && count < 256)
//			{
////				write(1, buf, count);
//				requestString += buf;
//				if (count != 255)
//					break ;
//				bzero(buf, 256);
//			}
//			try {
//				request = new Http::Request(requestString);
//				response = new Http::Response();
//				if (request->query_string.address == "/")
//				{
//					response
//						->body(file_get_contents("../public/image.html"))
//						->header("Content-Type", "text/html");
//				} else {
//					response->putFile(request->query_string.address);
//				}
//				result = send(sock_c, response->toString().data(),
//				      response->toString().length(), 0);
//
//				  if (result == -1) {
//				      // произошла ошибка при отправле данных
//				      cerr << "send failed" << endl;
//				  }
//				  delete response;
//			} catch (exception e) {
//
//			}
//			close(sock_c);
//		}
//		usleep(50);
//	}
//};

//void Server::closeServer();

void Server::StartListening()
{
	sockaddr_in client_addr;

	if (listen(socket_fd, serverConfig.clientLimit) < 0)
		return;

	cout << "Starting server: http://" << serverConfig.host << ":" << serverConfig.port << endl;

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
	Http::Request *request;
	Http::Response *response;

	FD_ZERO(&rfds);

	FD_SET(sock, &rfds);

	int retval = select(sock + 1, &rfds, NULL, NULL, &limitTime);

	if (retval && FD_ISSET(sock, &rfds)) {
		char buf[256];
		bzero(buf, 256);
		std::string requestString = "";
		int count;
		while ((count = read(sock, buf, 255)) > 0 && count < 256) {
			requestString += buf;
			if (count != 255)
				break;
			bzero(buf, 256);
		}
		try {
			request = new Http::Request(requestString);
			response = new Http::Response();
			if (request->query_string.address == "/") {
				response
						->body(file_get_contents(this->serverConfig.root_directory + "/image.html"))
						->header("Content-Type", "text/html");
			} else {
				response->putFile(this->serverConfig.root_directory + request->query_string.address);
			}
			result = send(sock, response->toString().data(), response->toString().length(), 0);
			if (result == -1) {
				// sending failed
				cerr << "send failed" << endl;
			}

			printLog(addr, "[" + to_string(response->code()) + "]: "
			               + request->query_string.method + " " + request->query_string.address);

			delete response;

		} catch (exception e) {
			// query string is incorrect
		}
	} else
		return;
}

bool Server::SendHttpResponse(const Http::Response)
{
	return true;
}