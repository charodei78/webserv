//
// Created by Holli Heimerdinger on 4/25/21.
//

#ifndef CGIREQUEST_HPP
#define CGIREQUEST_HPP

#include "includes.hpp"
#include "Request.hpp"
using namespace std;

namespace Http {
	class Request;
}

class CGIRequest
{
	CGIRequest();
	CGIRequest(CGIRequest const &rhs);

//	map<string, string> _env;
	vector<string>      _env;
	string              _cgi_path;

public:
	void makeQuery();
	CGIRequest(Http::Request &request, const Config& config, sockaddr_in client_addr);
	~CGIRequest();

	CGIRequest &operator=(CGIRequest const &rhs);

};


#endif //WEBSERV_CGIREQUEST_HPP
