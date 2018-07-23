/*
 * client_session.hpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#ifndef CLIENT_SESSION_HPP_
#define CLIENT_SESSION_HPP_

#include <sys/socket.h>
#include <arpa/inet.h>
#include "defines.hpp"


class ClientSession {
    struct sockaddr_in m_serverAddress;
    int m_serverSocket;
public:
    ClientSession();
    int UpdateAddress( const char* address );
    ReturnCode ConnectToServer();
};

#endif /* CLIENT_SESSION_HPP_ */
