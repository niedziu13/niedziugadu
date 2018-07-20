cmake_minimum_required (VERSION 3.0.0)
project (Niedziugadu)

set( PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set( CLIENT_PATH "${PROJECT_PATH}/Client")
set( SERVER_PATH "${PROJECT_PATH}/Server")

add_executable(Tutorial main.cpp)

add_subdirectory(Client)
add_subdirectory(Server)