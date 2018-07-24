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

enum ClientSessionStatus {
    ClientSessionStatus_Disconnected,
    ClientSessionStatus_Connceted,
    ClientSessionStatus_LogingPending,
    ClientSessionStatus_Logged
};

class ClientSession {
    struct sockaddr_in m_serverAddress;
    int m_serverSocket;
    ClientSessionStatus m_status;
public:
    ClientSession();
    int UpdateAddress( const char* address );
    ReturnCode ConnectToServer();
    ReturnCode Login();
};

#endif /* CLIENT_SESSION_HPP_ */
