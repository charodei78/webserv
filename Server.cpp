#include "Server.hpp"

void Server::printLog(sockaddr_in client_addr, const string &message = "")
{
	cout << "[" << getTimestamp() << "] "
	     << getIP(client_addr.sin_addr.s_addr) << ":" << client_addr.sin_port << " "
	     << message << endl;
}

Server::Server(const Config config) : serverConfig(config)
{
	serverName = config.domain;
}

Server::Server(Server const &rhs) : serverConfig(rhs.serverConfig)
{
	serverName = rhs.serverName;
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->socket_fd = rhs.socket_fd;
		this->serverName = rhs.serverName;
		this->serverConfig = rhs.serverConfig;
	}
	return *this;
}

string Server::GetServerName()
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


Http::Response  handleRequest()
{
	Http::Response response;





	return response;
}

void Server::SendAuthorizationRequest(const sockaddr_in &addr, const int sock)
{
    Http::Response response;

    response.code(401);
    response.statusText("Unauthorized");
    response.header("WWW-Authenticate", "Basic realm=\"Access to the staging site\", charset=\"UTF-8\"");
    string resStr = response.toString();
    send(sock, resStr.data(), resStr.length(), MSG_DONTWAIT);
    printLog(addr, "401 Unathorized");
}


bool Server::SendHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request, Config *config)
{
	int result;
	string message;
	Http::Response response;

	if (request->query.method == "PUT") {
		string path = config->rootDirectory + request->query.address;
		bool file_exists = exists(path);
		if (file_put_contents(path, request->body, 0666) == -1) {
			response.code(404);
		}
		else {
			if (file_exists)
				response.code(204);
			else
				response.code(201);
			response.header("Content-Location", request->query.address);
		}
	}
	else {
		string path = serverConfig.getIndexPath(request->query.address);

		if (path.empty())
			throw Http::http_exception(404, request->getLog(404), config);

		if (is_file(path)) {
			if (config->isCGI(path))
			{
				CGIRequest  cgiRequest(*request, serverConfig, addr);
				response = cgiRequest.makeQuery(request->body);
			} else {
				response.putFile(path);
			}
		}
		else if (config->metaVariables["autoindex"] == "on")
		{
			vector<string> *dirs = get_dir_content(path);
			string body;

			if (dirs == nullptr)
				throw Http::http_exception(500, request->getLog(500), config);

			string webPath = "<a href=\"" + request->query.address;
			if (*(--request->query.address.end()) != '/')
				webPath += '/';
			for (int i = 0; i < dirs->size(); i++) {
				body += webPath + (*dirs)[i] + "\">" + (*dirs)[i] + "</a><br>" ;
			}
			response.body(body);
			response.header("Content-Type", "text/html");
			delete dirs;
		}
		else
			throw Http::http_exception(403, request->getLog(403), config);
	}

	string resStr = response.toString();
	result = send(sock, resStr.data(), resStr.length(), MSG_DONTWAIT);
	if (result == -1) {
		// sending failed
		cerr << "send failed" << endl;
	}
	printLog(addr, request->getLog(response.code()));

	return true;
}

Server::Server()
{

}
