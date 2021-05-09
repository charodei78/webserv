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


bool Server::SendHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request)
{
	int result;
	string message;
	Http::Response response;

	try 
	{
		if (request->query.address == "/")
		{
			(&response)
					->body(file_get_contents(this->serverConfig.rootDirectory + "/" + this->serverConfig.index))
					->header("Content-Type", "text/html");
		}
		else if (request->query.address.find("php") != string::npos) {
			CGIRequest  cgiRequest(*request, serverConfig, addr);
			response = cgiRequest.makeQuery(request->body);
		} else {
			response.putFile(this->serverConfig.rootDirectory + request->query.address);
		}
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
