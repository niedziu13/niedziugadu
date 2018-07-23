/*
 * client_session.cpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#include "client_session.hpp"
#include "HAL_UI.hpp"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ClientSession::ClientSession(){
    memset( &m_serverAddress, 0, sizeof( m_serverAddress ) );
}

int ClientSession::UpdateAddress( const char* addr){
    m_serverAddress.sin_port = htons( SERVER_PORT );
    m_serverAddress.sin_family = AF_INET;
    return inet_aton(addr, &m_serverAddress.sin_addr);
}

ReturnCode ClientSession::ConnectToServer() {
    ReturnCode retVal = RET_OK;
    LOG_I( "Connecting to the server \n" );
    m_serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( m_serverSocket < 0 ) {
        LOG_E( "Socket error \n" );
        retVal = RET_SOCKET_ERROR;
    } else {
        if ( connect( m_serverSocket, ( struct sockaddr * ) &m_serverAddress, sizeof( m_serverAddress ) ) < 0 ) {
            LOG_E( "Error during connecting to the server\n" );
            retVal = RET_CONNECTION_ERROR;
        } else {
            LOG_I( "Connected to the server\n" );
        }
    }
    return retVal;
}
