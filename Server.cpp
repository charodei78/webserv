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

bool Server::SendHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request)
{
	int result;
	string message;
	Http::Response *response;
	CGIRequest  *cgiRequest;

	try 
	{
		response = new Http::Response();
		if (request->query.address == "/") 
		{
			response
					->body(file_get_contents(this->serverConfig.root_directory + "/image.html"))
					->header("Content-Type", "text/html");
		}
		else if (request->query.address.find("php") != string::npos) {
			cgiRequest = new CGIRequest(*request, serverConfig, addr);
			cgiRequest->makeQuery();
		} else {
			response->putFile(this->serverConfig.root_directory + request->query.address);
		}
		result = send(sock, response->toString().data(), response->toString().length(), 0);
		if (result == -1) {
			// sending failed
			cerr << "send failed" << endl;
		}
		message = "[" + to_string(response->code()) + "]: "
						+ request->query.method + " " + request->query.address;
		if (!request->query.query_string.empty())
			message += "?" + request->query.query_string;
		printLog(addr,  message);

		delete response;

	} catch (exception e) {
		// query string is incorrect
		printLog(addr,  strerror(errno));
		return false;
	}
	return true;
}