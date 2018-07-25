cmake_minimum_required (VERSION 3.0.0)

message("Building server")

set(SERVER_TARGET server)

set(CPP_COMPILE_FLAGS "-g -Wall -std=c++11")
set(CPP_LINK_FLAGS "-pthread")

set(SERVER_HEADERS 
	${PROJECT_PATH}/include/defines.hpp
	${SERVER_PATH}/include/server_session.hpp
	${SERVER_PATH}/include/server.hpp
	${SERVER_PATH}/include/server_logger.hpp
	)

set(SERVER_SOURCES 
    ${SERVER_PATH}/src/server.cpp
    ${SERVER_PATH}/src/server_session.cpp
    )

add_executable(${SERVER_TARGET} ${SERVER_SOURCES} ${SERVER_HEADERS})

target_include_directories(${SERVER_TARGET} PRIVATE ${PROJECT_PATH}/include)
target_include_directories(${SERVER_TARGET} PRIVATE ${SERVER_PATH}/include)

set_target_properties( ${SERVER_TARGET} PROPERTIES 
    COMPILE_FLAGS ${CPP_COMPILE_FLAGS}
    LINK_FLAGS ${CPP_LINK_FLAGS}
)
