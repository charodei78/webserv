#include "ServerListener.hpp"

void ServerListener::printLog(sockaddr_in client_addr, const string &message = "")
{
	cout << "[" << getTimestamp() << "] "
	     << getIP(client_addr.sin_addr.s_addr) << ":" << client_addr.sin_port << " "
	     << message << endl;
}

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
    sockaddr_in addr = sockaddr_in();
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	//Проверка удалось ли инициализировать сокет
	if (sock < 0 || bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		std::cerr << strerror(errno);
		return false;
	}
	std::cout << "Listener with " << port << " started\n";
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

		printLog(client_addr, "connected");
		try
		{
			ProcessConnectionToServer(client_addr, client_socket);
		}
		catch (std::exception)
		{
			cerr << strerror(errno);
		}
		close(client_socket);
	}
}


//TODO: Обработать запрос тут, и понять с какого сервера отправлять ответ. Главное запарсить хоста с реквеста сюда


//Запарсить домен, на который переходят, найти его в списке, если не нашелся то на первый в списке (дефолтный), 
//и уже в нем вызвать метод SendHttpResponse
void ServerListener::ProcessConnectionToServer(sockaddr_in client_addr, int client_socket)
{
	fd_set rfds;
	timeval limitTime;
	limitTime.tv_sec = 2;
	limitTime.tv_usec = 0;
	Http::Request *request;

	FD_ZERO(&rfds);

	FD_SET(client_socket, &rfds);

	int retval = select(client_socket + 1, &rfds, NULL, NULL, &limitTime);

	std::string requestString = "";
	if (retval && FD_ISSET(client_socket, &rfds)) 
	{
		char buf[256];
		bzero(buf, 256);
		int count;
		while ((count = read(client_socket, buf, 255)) > 0 && count < 256) 
		{
			requestString += buf;
			if (count != 255)
				break;
			bzero(buf, 256);
		}
	}
	else
		return ;

	request = new Http::Request(requestString);

	std::string host = request->headers["Host"];

    Server requiredServer = FindServerByHost(host);
    requiredServer.SendHttpResponse(client_addr, client_socket, request);
}

std::string tempHost;
bool IsServerNameEqualHost(Server server)
{
	tempHost.erase(tempHost.find_last_not_of(" \n\r\t")+1);
    return server.GetServerName() == tempHost;
}

Server &ServerListener::FindServerByHost(std::string host)
{
	tempHost = host;
	std::list<Server>::iterator serverIterator = std::find_if(servers.begin(), servers.end(), IsServerNameEqualHost);
    if (serverIterator == servers.end())
	{
		return *servers.begin();
	}
	else
	{
		return *serverIterator;
	}
}

//TODO: Обработать их тут, сразу задав в них сокеты этого сервера, чтоб сразу можно было использовать их в тех классах
bool ServerListener::BindServer(Server& server)
{
    if (std::find(servers.begin(), servers.end(), server) == servers.end())
    {
        servers.push_back(server);
        return true;
    }
    else
        return false;
}