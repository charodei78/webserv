#include "ServerListener.hpp"

void ServerListener::printLog(sockaddr_in client_addr, const string &message = "")
{
	cout << "[" << get_http_timestamp() << "] "
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

int ServerListener::getPort() const { return port; }

bool ServerListener::Intialize()
{
    sockaddr_in addr = sockaddr_in();
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	//Проверка удалось ли инициализировать сокет
	if (sock < 0) {
		cerr << strerror(errno);
		return false;
	}
	if (::bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        cerr << strerror(errno);
        return false;
    }

    if (listen(sock, 50) < 0)
        return false;

	cout << "Listener with " << port << " ready to listen\n";
	return true;
}

//Запарсить домен, на который переходят, найти его в списке, если не нашелся то на первый в списке (дефолтный), 
//и уже в нем вызвать метод SendHttpResponse

string tempHost;
bool IsServerNameEqualHost(Server server)
{
    tempHost = string(tempHost.begin(), find(tempHost.begin(), tempHost.end(), ':'));
	tempHost.erase(tempHost.find_last_not_of(" \n\r\t")+1);
    return server.GetServerName() == tempHost;
}

Server &ServerListener::FindServerByHost(string host)
{
	tempHost = host;
	list<Server>::iterator serverIterator = find_if(servers.begin(), servers.end(), IsServerNameEqualHost);
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
    if (find(servers.begin(), servers.end(), server) == servers.end())
    {
        servers.push_back(server);
        return true;
    }
    else
        return false;
}

ServerListener::ServerListener() : port(0){
    throw exception();
}

int ServerListener::getSock() {
    return sock;
}

Client ServerListener::acceptClient() {
    sockaddr_in client_addr;

    int addrlen = sizeof(client_addr);

    int client_socket = accept(sock, (struct sockaddr *) &client_addr,
                               reinterpret_cast<socklen_t *>(&addrlen));

    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    Client newClient(client_socket);

    return newClient;
}
