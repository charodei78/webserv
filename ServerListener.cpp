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

int ServerListener::getPort() { return port; }

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

    if (listen(sock, 10) < 0)
        return false;

    fcntl(sock, F_SETFL, O_NONBLOCK);

	cout << "Listener with " << port << " ready to listen\n";
	return true;
}

void ServerListener::StartListen()
{
    sockaddr_in client_addr;

	int addrlen = sizeof(client_addr);

    int client_socket = accept(sock, (struct sockaddr *) &client_addr,
                               reinterpret_cast<socklen_t *>(&addrlen));

    if (client_socket == -1 && errno == 35)
        return;
    else if (client_socket < 0) {
        cerr << strerror(errno);
        return;
    }


    printLog(client_addr, "connected");
    try
    {
        ProcessConnectionToServer(client_addr, client_socket);
    }
    catch (Http::http_exception &e) {
    	Http::Response  response;
	    response.code(e.code);
	    string message = e.what();
	    printLog(client_addr,  message);
	    string resStr = response.toString();
	    int result = send(client_socket, resStr.data(), resStr.length(), MSG_DONTWAIT);
	    if (result == -1) {
		    // sending failed
		    cerr << "send failed: " << errno << endl;
	    }
    }
    catch (exception)
    {
        cerr << strerror(errno);
        // 500;
    }
    close(client_socket);
}


//TODO: Обработать запрос тут, и понять с какого сервера отправлять ответ. Главное запарсить хоста с реквеста сюда


//Запарсить домен, на который переходят, найти его в списке, если не нашелся то на первый в списке (дефолтный), 
//и уже в нем вызвать метод SendHttpResponse
void ServerListener::ProcessConnectionToServer(sockaddr_in client_addr, int client_socket)
{
	Server requiredServer;

	fd_set rfds;
	timeval limitTime;
	limitTime.tv_sec = 2;
	limitTime.tv_usec = 0;

	Http::Request request;

	FD_ZERO(&rfds);
	FD_SET(client_socket, &rfds);

	int retval = select(client_socket + 1, &rfds, NULL, NULL, &limitTime);

	if (!retval || !FD_ISSET(client_socket, &rfds))
		return;


	request.parseQuery(readLine(client_socket));
	if (request.query.protocol != "1.1")
		throw Http::http_exception(505, request.getLog(505));
	request.parseHeaders(readBefore(client_socket, "\r\n\r\n", 1024));


	string host = request.headers["Host"];
	requiredServer = FindServerByHost(host);

	Config *config = &requiredServer.serverConfig;
	int BodyLimit = config->limitClientBodySize;

	if (!config->allowedFunctions.empty() && config->allowedFunctions.find(request.query.method) == -1)
		throw Http::http_exception(405, request.getLog(405), config);

	if (request.headers.count("Content-Length"))
		if (stoi(request.headers["Content-Length"]) > BodyLimit)
			throw Http::http_exception(413, request.getLog(413), config);

	unsigned long read_count;

	if (request.headers["Transfer-Encoding"] == "chunked")
	{
		while ((read_count = stoi(readLine(client_socket), nullptr, 16)) > 0) {
			request.body += readCount(client_socket, read_count);
			if (request.body.length() > BodyLimit)
				throw Http::http_exception(413, request.getLog(413), config);
		}
		clearStorage();
	}
	else
		request.body = readFull(client_socket);

    requiredServer.SendHttpResponse(client_addr, client_socket, &request);
}

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