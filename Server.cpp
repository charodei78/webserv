#include "Server.hpp"

Server::Server(const Config config) : serverConfig(config) {}

Server::Server(Server const &rhs) : serverConfig (rhs.serverConfig) {}

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
	if (socket_fd < 0 || bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
		std::cerr << strerror(errno);
        return false;
    }
    return true;
}

void Server::StartListening()
{
    if (listen(socket_fd, serverConfig.clientLimit) < 0)
        return;


    sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    for(;;)
    {
        int client_socket = accept(socket_fd, (struct sockaddr*)&client_addr, reinterpret_cast<socklen_t *>(&addrlen));
        if (client_socket == 0)
            continue;
        else
            if (client_socket < 0)
            {
                std::cerr << strerror(errno);
                return;
            }
        ProcessConnection(client_addr, client_socket);

        close(client_socket);
    }
}

void Server::ProcessConnection(const sockaddr_in &addr, const int sock)
{
    fd_set rfds;
    timeval limitTime;
    limitTime.tv_sec = 2;
    limitTime.tv_usec = 0;
    
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
            write(1, buf, count);
            requestString += buf;
            if (count != 255)
                break ;
            bzero(buf, 256);
        }
    }
    else
        return;
}

bool Server::SendHttpResponse(const Http::Response)
{
    return true;
}