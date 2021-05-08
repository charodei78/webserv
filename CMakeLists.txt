cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

add_executable(webserv main.cpp
        ServerEnvironment.cpp
        ServerEnvironment.hpp
        ConfigParser.cpp
        Config.hpp
        Config.cpp
        ConfigParser.hpp
        Helper.cpp
        Helper.hpp
        ENV.h
        includes.hpp
        Location.cpp
        Location.hpp
        Request.cpp
        Request.hpp
        Response.cpp
        Response.hpp
        Server.cpp
        Server.hpp
        ServerListener.cpp
        ServerListener.hpp
        CGIRequest.cpp
        CGIRequest.hpp http_exception.cpp http_exception.hpp)