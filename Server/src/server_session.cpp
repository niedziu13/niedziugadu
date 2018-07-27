/*
 * server_session.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#include "server_session.hpp"
#include "server_logger.hpp"
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

ServerSession::ServerSession() {
    memset( &m_serverAddress, 0, sizeof( m_serverAddress ) );
}

ServerSession::~ServerSession() {
    std::cout << "Server session destr" << std::endl;
    close( m_socket );
}

ReturnCode ServerSession::SetupAndListen() {
    ReturnCode retVal = RET_OK;
    LOG_I( "Setting up the server \n" );
    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( m_socket < 0 ) {
        LOG_E( "Socket error \n" );
        retVal = RET_SOCKET_ERROR;
    } else {
        m_serverAddress.sin_family = AF_INET;
        m_serverAddress.sin_port = htons( SERVER_PORT ); // check htons or ntohs
        m_serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
        if ( bind( m_socket, ( struct sockaddr * ) &m_serverAddress, sizeof( m_serverAddress ) ) ) {
            LOG_E( "Error during binding \n" );
            retVal = RET_SOCKET_ERROR;
        } else {
            listen( m_socket, MAX_QUEUED_USERS);
        }
    }
    return retVal;
}

int ServerSession::Accept() const{
    return accept(m_socket, 0, 0);
}
