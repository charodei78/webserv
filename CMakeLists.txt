cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

add_executable(webserv main.cpp Response.cpp Response.h ENV.h Server.cpp Server.hpp)

set_property(TARGET webserv PROPERTY CXX_STANDARD 98)

add_definitions(-std=c++11)