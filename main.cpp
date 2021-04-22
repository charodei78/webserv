 #include "Server.hpp"
 #include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Helper.hpp"
#include "Server.hpp"

int main()
{
//      Server server(80);
//      server.startServer();

//  vector<unsigned char > test = ;
//    ofstream stream("test2.png");
//  stream << file_get_contents("./test.png");
//stream.close();

//    Http::Response *response = new Http::Response;
//    response
//            ->code(200)
//            ->header("Cookie", "test")
//            ->header("User-Agent", "Human")
//            ->body("tested");
//    std::cout << response->toString();


    Config config;
    config.port = 80;
    config.clientLimit = 5;
    config.host = "127.0.0.1";
    config.root_directory = "../public";
    Server server1(config);
    if (server1.Intialize())
        server1.StartListening();
}