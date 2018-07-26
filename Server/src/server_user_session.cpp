/*
 * server_user_session.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: niedziu
 */

#include "server_user_session.hpp"
#include <string.h>
#include <unistd.h>

UserSession::UserSession( int sock ): m_sock( sock ) {
    memset( m_login, 0, LOGIN_MAX_SIZE );
    pthread_mutex_init( &m_readMutex, NULL );
    pthread_mutex_init( &m_writeMutex, NULL );
}

UserSession::~UserSession() {
    close( m_sock );
    pthread_mutex_destroy( &m_readMutex );
    pthread_mutex_destroy( &m_writeMutex );
}

int UserSessionDataBase::FindUserIndex( const char* login ) const {
    int ret = -1;
    for( unsigned j = 0; j < m_base.size() ; j++) {
        if( strcmp( m_base[j].GetLogin(), login) == 0 ) {
            ret = j;
        }
    }
    return ret;
}

UserSessionStatus UserSessionDataBase::GetUserStatus( const char* login ) const {
    UserSessionStatus retVal = UserSessionStatus_Invalid;
    int i;
    i = FindUserIndex( login );
    if ( i == -1 ) {
        retVal = UserSessionStatus_Logged;
    } else {
        retVal = m_base[i].GetStatus();
    }
    return retVal;
}

ReturnCode UserSession::GetMessage( Message& msg ) {
    ReturnCode retVal = RET_OK;
    pthread_mutex_lock( &m_readMutex );
    if ( m_readQueue.empty() ) {
        retVal = RET_BUFFOR_EMPTY;
    } else {
        // Assumes deep copy of all members
        msg = m_readQueue.front();
        m_readQueue.pop();
    }
    pthread_mutex_unlock( &m_readMutex );
    return retVal;
}

UserSessionStatus UserSession::GetStatus() const {
    return m_status;
}

const char* UserSession::GetLogin() const {
    return m_login;
}

int UserSession::GetSocket() const {
    return m_sock;
}
