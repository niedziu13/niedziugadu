cmake_minimum_required (VERSION 3.0.0)

message("Building server")

set(SERVER_TARGET server)

set(CPP_COMPILE_FLAGS "-g -Wall -std=c++11 -D ${SERVER_TARGET}")
set(CPP_LINK_FLAGS "-pthread")

set(SERVER_HEADERS 
	${PROJECT_PATH}/include/defines.hpp
	${PROJECT_PATH}/include/message.hpp
	${SERVER_PATH}/include/server_session.hpp
	${SERVER_PATH}/include/server.hpp
	${SERVER_PATH}/include/server_logger.hpp
	${SERVER_PATH}/include/server_user.hpp
	${SERVER_PATH}/include/server_memory.hpp
	${SERVER_PATH}/include/server_user_session.hpp
	${SERVER_PATH}/include/server_communication.hpp
	)

set(SERVER_SOURCES 
	${PROJECT_PATH}/src/message.cpp
    ${SERVER_PATH}/src/server.cpp
    ${SERVER_PATH}/src/server_session.cpp
    ${SERVER_PATH}/src/server_user.cpp
	${SERVER_PATH}/src/server_user_session.cpp
    )

add_executable(${SERVER_TARGET} ${SERVER_SOURCES} ${SERVER_HEADERS})

target_include_directories(${SERVER_TARGET} PRIVATE ${PROJECT_PATH}/include)
target_include_directories(${SERVER_TARGET} PRIVATE ${SERVER_PATH}/include)

set_target_properties( ${SERVER_TARGET} PROPERTIES 
    COMPILE_FLAGS ${CPP_COMPILE_FLAGS}
    LINK_FLAGS ${CPP_LINK_FLAGS}
)
