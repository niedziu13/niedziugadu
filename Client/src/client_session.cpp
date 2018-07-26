/*
 * client_session.cpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#include "client_session.hpp"
#include "client_communication.hpp"
#include "HAL_UI.hpp"
#include "HAL_memory.hpp"
#include "message.hpp"
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

ClientSession::ClientSession(){
    m_status = ClientSessionStatus_Disconnected;
    memset( &m_serverAddress, 0, sizeof( m_serverAddress ) );
    memset( m_login, 0, sizeof( m_login ) );
    pthread_mutex_init( &m_mutex, NULL );
}

ClientSession::~ClientSession(){
    pthread_mutex_destroy( &m_mutex );
    close( m_serverSocket );
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
            m_status = ClientSessionStatus_Connceted;
        }
    }
    return retVal;
}

ReturnCode ClientSession::Login() {
    ReturnCode retVal = RET_OK;
    LoggingPayload log_payload;
    Message log_msg;

    if ( m_status == ClientSessionStatus_Disconnected ) {
        LOG_E( "You are not connected to the server. Unable to login.\n" );
        retVal = RET_SESSION_STATUS_ERROR;
    } else if ( HAL_UI_GetLogin( log_payload.m_login ) != RET_OK ) {
        retVal = RET_INVALID_LOGIN;
    } else {
        memset( log_payload.m_passHASH, 1, HASH_SIZE);
        SavePayload( log_msg, log_payload );
        log_msg.m_header.m_len = static_cast<uint16_t>( sizeof( LoggingPayload ) );
        log_msg.m_header.m_type = MSGTYPE_LOGGING;
        retVal = SendMessage( log_msg, *this );
        if ( retVal != RET_OK ) {
            LOG_E( "Unable to send logging message.\n" );
        } else {
            strcpy( m_login, log_payload.m_login );
            m_status = ClientSessionStatus_LogingPending;
        }
    }
    return retVal;
}

int ClientSession::Lock( const timespec* time ) {
    return pthread_mutex_timedlock( &m_mutex, time );
}

int ClientSession::Unlock() {
    return pthread_mutex_unlock( &m_mutex );
}

int ClientSession::GetSocket() const {
    return m_serverSocket;
}
