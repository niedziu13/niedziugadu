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
    pthread_mutex_init( &m_sendMutex, NULL );
    pthread_mutex_init( &m_sessionMutex, NULL );
}

ClientSession::~ClientSession(){
    pthread_mutex_destroy( &m_sendMutex );
    pthread_mutex_destroy( &m_sendMutex );
//    close( m_serverSocket ); if not disconected?
}

int ClientSession::UpdateAddress( const char* addr){
    m_serverAddress.sin_port = htons( SERVER_PORT );
    m_serverAddress.sin_family = AF_INET;
    return inet_aton(addr, &m_serverAddress.sin_addr);
}

ReturnCode ClientSession::Connect() {
    ReturnCode retVal = RET_OK;
    LOG_I( "Connecting to the server \n" );
    if ( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( m_status != ClientSessionStatus_Disconnected ) {
            LOG_E( "You are not disconnected.\n" );
            retVal = RET_SESSION_STATUS_ERROR;
        } else {
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
        }
        if ( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}

ReturnCode ClientSession::Login() {
    ReturnCode retVal = RET_OK;
    LoggingReqPayload log_payload;
    Message log_msg;

    if ( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( m_status == ClientSessionStatus_Disconnected ) {
            LOG_E( "You are not connected to the server. Unable to login.\n" );
            retVal = RET_SESSION_STATUS_ERROR;
        } else if ( HAL_UI_GetLogin( log_payload.m_login ) != RET_OK ) {
            retVal = RET_INVALID_LOGIN;
        } else {
            memset( log_payload.m_passHASH, 1, HASH_SIZE);
            SavePayload( log_msg, log_payload );
            log_msg.m_header.m_len = static_cast<uint16_t>( sizeof( LoggingReqPayload ) );
            log_msg.m_header.m_type = MSGTYPE_LOGGING_REQ;
            retVal = SendMessage( log_msg, *this );
            if ( retVal != RET_OK ) {
                LOG_E( "Unable to send logging message.\n" );
            } else {
                strcpy( m_login, log_payload.m_login );
                m_status = ClientSessionStatus_LogingPending;
            }
            if ( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
                retVal = RET_MUTEX_ERROR;
            }
        }
    }
    return retVal;
}

int ClientSession::LockSend( const timespec* time ) {
    return pthread_mutex_timedlock( &m_sendMutex, time );
}

int ClientSession::UnlockSend() {
    return pthread_mutex_unlock( &m_sendMutex );
}

int ClientSession::LockSession() {
    return pthread_mutex_lock( &m_sessionMutex );
}

int ClientSession::UnlockSession() {
    return pthread_mutex_unlock( &m_sessionMutex );
}

ClientSessionStatus ClientSession::GetStatus() const {
    return m_status;
}

void ClientSession::SetStatus( ClientSessionStatus status ) {
    m_status = status;
}


const char* ClientSession::GetLogin() const {
    return m_login;
}

int ClientSession::GetSocket() const {
    return m_serverSocket;
}

ReturnCode ClientSession::Close() {
    ReturnCode retVal = RET_OK;
    if ( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( pthread_mutex_lock( &m_sendMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        } else {
            m_status = ClientSessionStatus_Disconnected;
            close( m_serverSocket );
            LOG_I( "Connection closed.\n");
            if ( pthread_mutex_unlock( &m_sendMutex ) != 0 ) {
                retVal = RET_MUTEX_ERROR;
            }
        }
        if ( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}

ReturnCode ClientSession::Disconnect() {
    ReturnCode retVal = RET_OK;
    if ( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( pthread_mutex_lock( &m_sendMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        } else {
            m_status = ClientSessionStatus_Disconnected;
            shutdown( m_serverSocket, SHUT_RDWR );
            LOG_I( "Disconnected.\n");
            if ( pthread_mutex_unlock( &m_sendMutex ) != 0 ) {
                retVal = RET_MUTEX_ERROR;
            }
        }
        if ( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}
