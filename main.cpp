#include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Helper.hpp"
#include "Server.hpp"

int main()
{
	Server server(80);
	server.startServer();

//  vector<unsigned char > test = ;
//	ofstream stream("test2.png");
//  stream << file_get_contents("./test.png");
//stream.close();

//	Http::Response *response = new Http::Response;
//	response
//		->code(200)
//		->header("Cookie", "test")
//		->header("User-Agent", "Human")
//		->body("tested");
//	std::cout << response->toString();


	return 0;
}
