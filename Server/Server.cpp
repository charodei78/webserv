#include <list>
#include "Server.hpp"

void Server::printLog(sockaddr_in client_addr, const string &message = "")
{
	static int counter = 0;
    cout << ++counter
		<< " [" << get_http_timestamp() << "] "
         << getIP(client_addr.sin_addr.s_addr) << ":" << client_addr.sin_port << " "
         << message << endl;
}

Server::Server(Config config) : serverConfig(config)
{
	serverName = config.domain;
	vector<Location>::iterator begin = config.locations.begin();
    while (begin != config.locations.end())
    {
        Location &location = *begin;
        locations[location] = Server(*location.config);
        begin++;
    }
}

Server::Server(Server const &rhs) : serverConfig(rhs.serverConfig)
{
	serverName = rhs.serverName;
	locations = rhs.locations;
}

Server &Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->serverName = rhs.serverName;
		this->serverConfig = rhs.serverConfig;
		this->locations = rhs.locations;
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
    response.header("WWW-Authenticate", "(Basic realm=\"Access to the staging site\", charset=\"UTF-8\")");
    string resStr = response.toString();
    send(sock, resStr.data(), resStr.length(), MSG_DONTWAIT);
    printLog(addr, "401 Unathorized");
}

Server &Server::GetLocationServer(string &uri)
{
    map<Location, Server>::iterator     locationsIter = locations.begin();
	map<Location, Server>::iterator     coincidence = locations.end();
	int maxExtLength = -1;


	while (locationsIter != locations.end())
    {
    	if (locationsIter->first.path == uri)
	    {
		    coincidence = locationsIter;
		    break;
	    }
    	string path = locationsIter->first.path;
    	if (path.substr(path.length() - 2) == "/*")
	    {
			if (path.substr(0, path.length() - 2) == uri)
			{
				coincidence = locationsIter;
				break;
			}
	    }
        if (match(uri.c_str() ,path.c_str()))
        {
        	int tmp = locationsIter->first.path.length() - locationsIter->first.path.find_last_of('*');
        	if (tmp > maxExtLength || maxExtLength == -1 || (tmp == maxExtLength && coincidence->first.path.length() < locationsIter->first.path.length())) {
		        coincidence = locationsIter;
		        maxExtLength = tmp;
	        }
        }
        locationsIter++;
    }
	if (coincidence != locations.end()) {
		string path = coincidence->first.path;
		int lastDirIndex = path.find_last_of('/');

		if (lastDirIndex > 0) {
			uri.erase(0, lastDirIndex);
		}
		return coincidence->second;
	}
    return *this;
}

Http::Response Server::getHttpResponse(const sockaddr_in &addr, const int sock, Http::Request *request, Config *config)
{
	(void)sock;
    string message;
    string path;
    Http::Response response;

	if (config->isCGI(path))
	{
		CGIRequest  cgiRequest(*request, serverConfig, addr);
		response = cgiRequest.makeQuery(request->file_fd);
		response.isCgi = true;
		return response;
	}
    if (request->query.method == "PUT" || request->query.method == "POST") {
        path = config->rootDirectory + request->query.address;
        bool file_exists = exists(path);
		int result;


		if (request->file_fd)
			result = file_put_contents(path, request->file_fd, 0666);
		else
			result = file_put_contents(path, request->body, 0666);
		if (result == -1) {
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
        path = serverConfig.getIndexPath(request->query.address);


	    if (path.empty())
		    return *response.code(404);
	    else if (is_file(path)){
            response.putFile(path);
        }
        else if (config->autoindex)
        {
            vector<string> *dirs = get_dir_content(path);
            string body;
            if (dirs == nullptr)
                return *response.code(500);
            string webPath = "<a href=\"" + request->query.address;
            if (*(--request->query.address.end()) != '/')
                webPath += '/';
            for (unsigned i = 0; i < dirs->size(); i++) {
                body += webPath + (*dirs)[i] + "\">" + (*dirs)[i] + "</a><br>" ;
            }
            response.body(body);
            response.header("Content-Type", "text/html");
            delete dirs;
        }
        else
            return *response.code(404);
    }
    response.attachDefaultHeaders(*config);
    return response;
}

Server::Server()
{

}
