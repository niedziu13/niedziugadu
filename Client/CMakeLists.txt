cmake_minimum_required (VERSION 3.0.0)

message("Building client")

set(CLIENT_TARGET client)

set(CPP_COMPILE_FLAGS "-g -Wall -std=c++11 -D ${CLIENT_TARGET}")
set(CPP_LINK_FLAGS "-pthread")

set(CLIENT_HEADERS 
	${CLIENT_PATH}/include/client_menu.hpp
	${CLIENT_PATH}/include/client_session.hpp
	${CLIENT_PATH}/include/client_communication.hpp
	${CLIENT_PATH}/include/client_commands.hpp
	${CLIENT_PATH}/include/client.hpp
	${CLIENT_PATH}/include/HAL/HAL_UI.hpp
	${CLIENT_PATH}/include/HAL/HAL_memory.hpp
	${PROJECT_PATH}/include/defines.hpp
	${PROJECT_PATH}/include/message.hpp
	)

set(CLIENT_SOURCES 
    ${CLIENT_PATH}/src/client.cpp
    ${CLIENT_PATH}/src/client_menu.cpp
    ${CLIENT_PATH}/src/client_session.cpp
    ${CLIENT_PATH}/src/client_commands.cpp
    ${CLIENT_PATH}/src/HAL/HAL_UI.cpp
    ${CLIENT_PATH}/src/HAL/HAL_memory.cpp
	${PROJECT_PATH}/src/message.cpp
    )

add_executable(${CLIENT_TARGET} ${CLIENT_SOURCES} ${CLIENT_HEADERS})

target_include_directories(${CLIENT_TARGET} PRIVATE ${PROJECT_PATH}/include)
target_include_directories(${CLIENT_TARGET} PRIVATE ${CLIENT_PATH}/include)
target_include_directories(${CLIENT_TARGET} PRIVATE ${CLIENT_PATH}/include/HAL)

set_target_properties( ${CLIENT_TARGET} PROPERTIES 
    COMPILE_FLAGS ${CPP_COMPILE_FLAGS}
    LINK_FLAGS ${CPP_LINK_FLAGS}
)
