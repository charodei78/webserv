//
// Created by Holli Heimerdinger on 4/25/21.
//

#ifndef WEBSERV_CGIREQUEST_HPP
#define WEBSERV_CGIREQUEST_HPP

#include "includes.hpp"
using namespace std;

class CGIRequest
{
	CGIRequest();
	CGIRequest(CGIRequest const &rhs);

	map<string, string> _env;

public:
//	string makeQuery();
	CGIRequest(Http::Request &request, const Config& config, sockaddr_in client_addr);
	~CGIRequest();

	CGIRequest &operator=(CGIRequest const &rhs);

};


#endif //WEBSERV_CGIREQUEST_HPP
