#include <iostream>
#include "Response.h"

int main()
{
	Http::Response *response = new Http::Response;

	response
		->code(200)
		->header("Cookie", "test")
		->header("User-Agent", "Human")
		->body("tested");
	std::cout << response->toString();
	return 0;
}
