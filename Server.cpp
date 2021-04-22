//
// Created by Holli Heimerdinger on 4/18/21.
//

#include "Server.hpp"

Server::Server(unsigned port): port(port)
{
}

void Server::startServer()
{
	sockaddr_in addr = sockaddr_in();
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	this->socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (this->socket_fd < 0
	    || bind(this->socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0
	    || listen(this->socket_fd, SOMAXCONN) < 0)
		cerr << strerror(errno);

	this->handle();
};

_Noreturn void Server::handle()
{
	int         sock_c;
	sockaddr_in addr_c;
	int         addrlen;
	int         result;
	string      requestString;
	Http::Request     *request;
	Http::Response    *response;
	unsigned    count;
	char        buf[256];

	while (1)
	{
		addrlen = sizeof addr_c;
		if ((sock_c = accept(this->socket_fd, (struct sockaddr*)&addr_c, reinterpret_cast<socklen_t *>(&addrlen))) != 0)
		{
			bzero(buf, 256);
			requestString = "";
			while ((count = read(sock_c, buf, 255)) > 0 && count < 256)
			{
//				write(1, buf, count);
				requestString += buf;
				if (count != 255)
					break ;
				bzero(buf, 256);
			}
			try {
				request = new Http::Request(requestString);
				response = new Http::Response();
				if (request->query_string.address == "/")
				{
					response
						->body(file_get_contents("../Tests/image.html"))
						->header("Content-Type", "text/html");
				} else {
					response->putFile(request->query_string.address);
				}
				result = send(sock_c, response->toString().data(),
				      response->toString().length(), 0);

				  if (result == -1) {
				      // произошла ошибка при отправле данных
				      cerr << "send failed" << endl;
				  }
				  delete response;
			} catch (exception e) {
				
			}
			close(sock_c);
		}
		usleep(50);
	}
};

void Server::closeServer()
{

	if (this->socket_fd > 2)
		close(this->socket_fd);

};

Server::Server(Server const &rhs)
{
	operator=(rhs);
};

Server::~Server()
{
};

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->port = rhs.port;
	}
	return *this;
}
