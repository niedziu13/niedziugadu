cmake_minimum_required (VERSION 3.0.0)

message("Building client")

set(CLIENT_TARGET client)

set(CPP_COMPILE_FLAGS "-g -Wall -std=c++11")
set(CPP_LINK_FLAGS "")

set(CLIENT_HEADERS 
	${CLIENT_PATH}/include/client_menu.hpp
	${CLIENT_PATH}/include/client_session.hpp
	${CLIENT_PATH}/include/client.hpp
	${CLIENT_PATH}/include/HAL/HAL_UI.hpp
	${PROJECT_PATH}/include/defines.hpp)

set(CLIENT_SOURCES 
    ${CLIENT_PATH}/src/client_menu.cpp
    ${CLIENT_PATH}/src/client_session.cpp
    ${CLIENT_PATH}/src/HAL/HAL_UI.cpp
    ${CLIENT_PATH}/src/client.cpp)

add_executable(${CLIENT_TARGET} ${CLIENT_SOURCES} ${CLIENT_HEADERS})

target_include_directories(${CLIENT_TARGET} PRIVATE ${PROJECT_PATH}/include)
target_include_directories(${CLIENT_TARGET} PRIVATE ${CLIENT_PATH}/include)
target_include_directories(${CLIENT_TARGET} PRIVATE ${CLIENT_PATH}/include/HAL)

set_target_properties( ${CLIENT_TARGET} PROPERTIES 
    COMPILE_FLAGS ${CPP_COMPILE_FLAGS}
)
