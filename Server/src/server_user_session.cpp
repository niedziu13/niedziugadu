/*
 * server_user_session.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: niedziu
 */

#include "server_user_session.hpp"
#include "server_logger.hpp"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

UserSession::UserSession()
{
    m_ongoingWrites = 0;
    m_sessionStatus = UserSessionStatus_Inactive;
    memset( m_login, 0, LOGIN_MAX_SIZE );
    pthread_mutex_init( &m_sendMutex, NULL );
    pthread_mutex_init( &m_sessionMutex, NULL );
}

UserSession::~UserSession() {
//    close( m_sock );
    pthread_mutex_destroy( &m_sendMutex );
    pthread_mutex_destroy( &m_sessionMutex );
}


UserSessionStatus UserSession::GetStatus() {
    UserSessionStatus retVal = UserSessionStatus_Inactive;
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        LOG_E( " Error getting status " );
        retVal = UserSessionStatus_TearDown;
    } else {
        retVal = m_sessionStatus;
    }
    pthread_mutex_unlock( &m_sessionMutex );
    return retVal;
}

ReturnCode UserSession::Reserve( int sock ) {
    ReturnCode retVal = RET_OK;
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        LOG_E( " Error getting status " );
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( m_sessionStatus != UserSessionStatus_Inactive ) {
            retVal = RET_SESSION_STATUS_ERROR;
        } else {
            m_sessionStatus = UserSessionStatus_Connected;
            m_sock = sock;
        }
    }
    pthread_mutex_unlock( &m_sessionMutex );
    return retVal;
}

void UserSession::SetStatus( UserSessionStatus status ) {
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        LOG_E( " Error getting status " );
    } else {
        m_sessionStatus = status;
    }
    pthread_mutex_unlock( &m_sessionMutex );
}

void UserSession::SetLogin( const char* login ) {
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        LOG_E( " Error setting status " );
    } else {
        strcpy( m_login, login );
    }
    pthread_mutex_unlock( &m_sessionMutex );
}

const char* UserSession::GetLogin() const {
    return m_login;
}

int UserSession::GetSocket() const {
    return m_sock;
}

ReturnCode UserSession::LockSend( const timespec* time ) {
    ReturnCode retVal = RET_OK;
    if( pthread_mutex_timedlock( &m_sendMutex, time ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    }
    return retVal;
}

ReturnCode UserSession::UnlockSend() {
    ReturnCode retVal = RET_OK;
    if ( ( pthread_mutex_unlock( &m_sendMutex ) != 0 ) ) {
        retVal = RET_MUTEX_ERROR;
    }
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( --m_ongoingWrites < 0 ) {
            m_ongoingWrites = 0;
            LOG_E( "Invalid ongoingWrites number\n");
        }
        if( pthread_cond_signal( &m_cond ) != 0 ) {
            retVal = RET_VAR_COND_ERROR;
        }
        if( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}

ReturnCode UserSession::LockComingSend( const char* login ) {
    ReturnCode retVal = RET_OK;
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( m_sessionStatus != UserSessionStatus_Logged ) {
            retVal = RET_SESSION_STATUS_ERROR;
        } else {
            if ( login != NULL ) {
                if(strcmp( m_login, login ) != 0 ) {
                    retVal = RET_INVALID_LOGIN;
                }
            }
            if ( retVal == RET_OK ) {
                ++m_ongoingWrites;
            }
        }
        pthread_mutex_unlock( &m_sessionMutex );
    }
    return retVal;
}

ReturnCode UserSession::Close() {
    ReturnCode retVal = RET_OK;
    if( pthread_mutex_lock( &m_sessionMutex ) != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        m_sessionStatus = UserSessionStatus_TearDown;
        while( m_ongoingWrites != 0 ) {
            pthread_cond_wait( &m_cond, &m_sessionMutex );
        }
        shutdown( m_sock, SHUT_RDWR );
        close( m_sock );
        m_sessionStatus = UserSessionStatus_Inactive;
        if ( pthread_mutex_unlock( &m_sessionMutex ) != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}

//int UserSessionDataBase::FindUserIndex( const char* login ) const {
//    int ret = -1;
//    for( unsigned j = 0; j < m_base.size() ; j++) {
//        if( strcmp( m_base[j].GetLogin(), login) == 0 ) {
//            ret = j;
//        }
//    }
//    return ret;
//}

//UserConnectionStatus UserSessionDataBase::GetUserStatus( const char* login ) const {
//    UserConnectionStatus retVal = UserSessionStatus_Invalid;
//    int i;
//    i = FindUserIndex( login );
//    if ( i == -1 ) {
//        retVal = UserSessionStatus_Logged;
//    } else {
//        retVal = m_base[i].GetStatus();
//    }
//    return retVal;
//}

UserSessionDataBase::UserSessionDataBase() {
    m_lastReturnedSessionIndex = 0;
}

UserSession* UserSessionDataBase::GetToSend( const char* login ) {
    UserSession* retVal = NULL;
    for( unsigned i = 0; i < MAX_USERS_CONNECTED ; i++ ) {
        if( m_base[i].LockComingSend( login ) == RET_OK ) {
            retVal = &m_base[i];
            break;
        }
    }
    return retVal;
}

int UserSessionDataBase::GetToSend( int index ) {
    int ret = -1;
    if( m_base[index].LockComingSend( NULL ) == RET_OK ) {
        ret = index;
    }
    return ret;
}

UserSession* UserSessionDataBase::GetNewSession( int sock ) {
    UserSession* retVal = NULL;
    for( unsigned i = 0; i < MAX_USERS_CONNECTED; i++) {
        if( ++m_lastReturnedSessionIndex >= MAX_USERS_CONNECTED ) {
            m_lastReturnedSessionIndex = 0;
        }
        if ( m_base[ m_lastReturnedSessionIndex ].Reserve( sock ) == RET_OK ) {
            retVal = &m_base[ m_lastReturnedSessionIndex ];
            break;
        }
    }
    return retVal;
}

//ReturnCode UserSession::GetMessage( Message& msg ) {
//    ReturnCode retVal = RET_OK;
//    pthread_mutex_lock( &m_readMutex );
//    if ( m_readQueue.empty() ) {
//        retVal = RET_BUFFOR_EMPTY;
//    } else {
//        // Assumes deep copy of all members
//        msg = m_readQueue.front();
//        m_readQueue.pop();
//    }
//    pthread_mutex_unlock( &m_readMutex );
//    return retVal;
//}
