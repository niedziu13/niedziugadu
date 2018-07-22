/*
 * client_session.hpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#ifndef CLIENT_SESSION_HPP_
#define CLIENT_SESSION_HPP_

#include <sys.socket.h>

class ClientSession {
    sockaddr_in m_serverAddress;
public:
    ClientSession();
    int UpdateAddress( char* address );
};

#endif /* CLIENT_SESSION_HPP_ */
