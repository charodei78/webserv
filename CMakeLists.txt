cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

add_executable(webserv main.cpp
        Server/ServerCluster.cpp
        Server/ServerCluster.hpp
        Server/Config/ConfigParser.cpp
        Server/Config/Config.hpp
        Server/Config/Config.cpp
        Server/Client.hpp
        Server/Client.cpp
        Server/Config/ConfigParser.hpp
        Helpers/Helper.cpp
        Helpers/Reader.cpp
        Helpers/Reader.hpp
        Helpers/Helper.hpp
        ENV.h
        includes.hpp
        Server/Config/Location.cpp
        Server/Config/Location.hpp
        HTTP/Request.cpp
        HTTP/Request.hpp
        HTTP/Response.cpp
        HTTP/Response.hpp
        Server/Server.cpp
        Server/Server.hpp
        Server/ServerListener.cpp
        Server/ServerListener.hpp
        HTTP/CGIRequest.cpp
        HTTP/CGIRequest.hpp exceptions/http_exception.cpp exceptions/http_exception.hpp exceptions/file_not_found_exception.cpp exceptions/file_not_found_exception.hpp exceptions/webserv_exceptions.hpp Server/RequestParser.cpp Server/RequestParser.hpp)