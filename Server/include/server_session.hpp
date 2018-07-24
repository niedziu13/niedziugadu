/*
 * server_session.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef SERVER_SESSION_HPP_
#define SERVER_SESSION_HPP_

#include <sys/socket.h>
#include <arpa/inet.h>
#include "defines.hpp"

#define MAX_QUEUED_USERS 10U

class ServerSession {
    struct sockaddr_in m_serverAddress;
    int m_socket;
public:
    ServerSession();
    ReturnCode SetupAndListen();
    int Accept() const;
};

#endif /* SERVER_SESSION_HPP_ */
