cmake_minimum_required (VERSION 3.0.0)

message("Building server")

set(SERVER_TARGET server)

set(CPP_COMPILE_FLAGS "-g -Wall")
set(CPP_LINK_FLAGS "")

set(SERVER_HEADERS 
	${PROJECT_PATH}/include/defines.hpp)

set(SERVER_SOURCES 
    ${SERVER_PATH}/src/server.cpp)

add_executable(${SERVER_TARGET} ${SERVER_SOURCES} ${SERVER_HEADERS})

target_include_directories(${SERVER_TARGET} PRIVATE ${PROJECT_PATH}/include)

set_target_properties( ${SERVER_TARGET} PROPERTIES 
    COMPILE_FLAGS ${CPP_COMPILE_FLAGS}
)
