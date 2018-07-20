cmake_minimum_required (VERSION 3.0.0)

message("Building server")

set(SERVER_SOURCES 
    ${SERVER_PATH}/src/server.cpp)

add_executable(server ${SERVER_SOURCES})