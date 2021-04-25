#include "ServerListener.hpp"

ServerListener::ServerListener(const int port) : port(port)
{
}

ServerListener::~ServerListener()
{
    if (sock > 2)
        close(sock);
}

bool ServerListener::Intialize()
{
    return true;
}

void ServerListener::StartListen()
{
    sockaddr_in client_addr;

	if (listen(sock, 10) < 0)
		return;

	int addrlen = sizeof(client_addr);

	for (;;) {
		int client_socket = accept(sock, (struct sockaddr *) &client_addr,
		                           reinterpret_cast<socklen_t *>(&addrlen));
		if (client_socket == 0)
			continue;
		else {
			if (client_socket < 0) {
				cerr << strerror(errno);
				return;
			}
		}

		ProcessConnectionToServer(client_addr, client_socket);

		close(client_socket);
	}
}


//TODO: Обработать запрос тут, и понять с какого сервера отправлять ответ. Главное запарсить хоста с реквеста сюда
std::string host;

//Запарсить домен, на который переходят, найти его в списке, если не нашелся то на первый в списке (дефолтный), 
//и уже в нем вызвать метод SendHttpResponse
void ServerListener::ProcessConnectionToServer(sockaddr_in client_addr, int client_socket)
{
    Server requiredServer = FindServerByHost(servers);
    requiredServer.ProcessConnection(client_addr, client_socket);
}

bool IsServerNameEqualHost(Server server)
{
    return server.GetServerName() == host;
}

Server &FindServerByHost(std::list<Server> serverList)
{
    return *std::find_if(serverList.begin(), serverList.end(), IsServerNameEqualHost);
}

//TODO: Обработать их тут, сразу задав в них сокеты этого сервера, чтоб сразу можно было использовать их в тех классах
bool ServerListener::BindServer(Server& server)
{
    if (std::find(servers.begin(), servers.end(), server) == servers.end())
    {
        // servers.push_back(server);
        return true;
    }
    else
        return false;
}