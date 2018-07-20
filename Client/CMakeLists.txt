cmake_minimum_required (VERSION 3.0.0)

message("Building client")

set(CLIENT_SOURCES 
    ${CLIENT_PATH}/src/client.cpp)

add_executable(client ${CLIENT_SOURCES})