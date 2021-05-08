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


bool Server::SendHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request, Config *config)
{
	int result;
	string message;
	Http::Response response;

	try 
	{
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

		string resStr = response.toString();
		result = send(sock, resStr.data(), resStr.length(), MSG_DONTWAIT);
		if (result == -1) {
			// sending failed
			cerr << "send failed" << endl;
		}
		printLog(addr, request->getLog(response.code()));

	} catch (exception e) {
		// query string is incorrect
		printLog(addr,  strerror(errno));
		return false;
	}
	return true;
}

Server::Server()
{

}
