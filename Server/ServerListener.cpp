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

    if (listen(sock, 10) < 0)
        return false;

    fcntl(sock, F_SETFL, O_NONBLOCK);

	cout << "Listener with " << port << " ready to listen\n";
	return true;
}

void ServerListener::StartListen()
{
	Http::Response  response;
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

	string error_page;

	printLog(client_addr, "connected");
    try
    {
        ProcessConnectionToServer(client_addr, client_socket);
	    close(client_socket);
	    return;
    }
    catch (Http::http_exception &e) {
    string message;

	    response.code(e.code);
	    if (e.config)
		    error_page = e.config->errorPage;
	    if (error_page.empty())
		    error_page = DEFAULT_ERROR_PAGE;
	    printLog(client_addr,  e.what());
	    response.attachDefaultHeaders(*e.config);
    } catch (exception &e) {
	    response.code(500);
	    printLog(client_addr,  e.what());
	    error_page = DEFAULT_ERROR_PAGE;
    }
    try {
	    response.putFile(error_page);
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    string resStr = response.toString();
    int result = send(client_socket, resStr.data(), resStr.length(), MSG_DONTWAIT);
    if (result == -1) {
	    // sending failed
	    cerr << "send failed: " << errno << endl;
    }
}


//TODO: Обработать запрос тут, и понять с какого сервера отправлять ответ. Главное запарсить хоста с реквеста сюда


//Запарсить домен, на который переходят, найти его в списке, если не нашелся то на первый в списке (дефолтный), 
//и уже в нем вызвать метод SendHttpResponse
void ServerListener::ProcessConnectionToServer(sockaddr_in client_addr, int client_socket)
{
	Server requiredServer;
	Reader reader;

	fd_set rfds;
	timeval limitTime;
	limitTime.tv_sec = 10;
	limitTime.tv_usec = 0;

	Http::Request request;

	FD_ZERO(&rfds);
	FD_SET(client_socket, &rfds);

	int retval = select(client_socket + 1, &rfds, nullptr, nullptr, &limitTime);

	if (!retval || !FD_ISSET(client_socket, &rfds))
		return;


	request.parseQuery(reader.readLine(client_socket));
	if (request.query.protocol != "1.1")
		throw Http::http_exception(505, request.getLog(505));
	request.parseHeaders(reader.readBefore(client_socket, "\r\n\r\n", 1024));


	string host = request.headers["Host"];
	requiredServer = FindServerByHost(host);
	requiredServer = requiredServer.GetLocationServer(request.query.address);

	Config *config = &requiredServer.serverConfig;
	int BodyLimit = config->limitClientBodySize;

	if (config->auth)
	{
	    if (request.headers.count("Authorization"))
	    {
	        string authEncoded = request.headers["Authorization"];
	        authEncoded = string(find(authEncoded.begin(), authEncoded.end(), ' ') + 1, authEncoded.end());
	        if (find(config->auth_file_content.begin(), config->auth_file_content.end(), authEncoded) == config->auth_file_content.end())
	        {
                requiredServer.SendAuthorizationRequest(client_addr, client_socket);
                return;
            }
	        //TODO: Проверить правильны ли пришедшие данные, с файла с авторизациями
	    }
        else
        {
            requiredServer.SendAuthorizationRequest(client_addr, client_socket);
	        return;
        }
	}

	if (!config->allowedFunctions.empty() && config->allowedFunctions.find(request.query.method) == -1)
		throw Http::http_exception(405, request.getLog(405), config);

	if (request.headers.count("Content-Length"))
		if (stoi(request.headers["Content-Length"]) > BodyLimit)
			throw Http::http_exception(413, request.getLog(413), config);

	unsigned long read_count;

	if (request.headers["Transfer-Encoding"] == "chunked")
	{
		while ((read_count = stoi(reader.readLine(client_socket), nullptr, 16)) > 0) {
			request.body += reader.readCount(client_socket, read_count);
			if (request.body.length() > BodyLimit)
				throw Http::http_exception(413, request.getLog(413), config);
		}
	}
	else {
		if (request.headers.count("Content-Length"))
			request.body = reader.readCount(client_socket, stoi(request.headers["Content-Length"]));
		else
			request.body = reader.readFull(client_socket);
	}
	requiredServer.SendHttpResponse(client_addr, client_socket, &request, config);
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

ServerListener::ServerListener() : port(0){
    throw exception();
}
