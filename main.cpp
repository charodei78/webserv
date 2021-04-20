#include <iostream>
#include "Response.hpp"
#include "Server.hpp"

int main()
{
	Server server(80);
	server.startServer();

//	Http::Response *response = new Http::Response;
//	response
//		->code(200)
//		->header("Cookie", "test")
//		->header("User-Agent", "Human")
//		->body("tested");
//	std::cout << response->toString();
	return 0;
}
