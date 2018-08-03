/*
 * server_user_session.hpp
 *
 *  Created on: Jul 25, 2018
 *      Author: niedziu
 */

#ifndef SERVER_USER_SESSION_HPP_
#define SERVER_USER_SESSION_HPP_

#include "message.hpp"
#include <queue>
#include <pthread.h>
#include <unistd.h>

#define MAX_USERS_CONNECTED 10U

//enum UserConnectionStatus {
//    UserConnectionStatus_Logged,
//    UserConnectionStatus_Connected,
//    UserConnectionStatus_Invalid
//};

enum UserSessionStatus {
    UserSessionStatus_Logged,
    UserSessionStatus_Connected,
    UserSessionStatus_TearDown,
    UserSessionStatus_Inactive
};

class UserSession: public Session {
    char m_login[LOGIN_MAX_SIZE];
    pthread_mutex_t m_writeMutex;
    int m_sock;
    unsigned m_ongoingWrites;
    UserSessionStatus m_sessionStatus;
    pthread_cond_t m_cond;
    pthread_mutex_t m_sessionMutex; // Mutex of session status including ongoing writes
    pthread_mutex_t m_sendMutex; // Mutex of session status including ongoing writes
public:
    UserSession();
    ~UserSession();
    ReturnCode GetMessage( Message& msg );
    ReturnCode SendMessage( const Message& msg );
    void SetStatus( UserSessionStatus status );
    UserSessionStatus GetStatus();
    void SetLogin( const char* login );
    const char* GetLogin() const;
    int GetSocket() const;
    ReturnCode Reserve( int sock );
    int LockSend( const timespec* time );
    int UnlockSend();
    ReturnCode LockComingSend( const char* login );
    ReturnCode Close();
};

class UserSessionDataBase {
    UserSession m_base[MAX_USERS_CONNECTED];
    unsigned m_lastReturnedSessionIndex;
    unsigned FindUserIndex( const char* login ) const;
public:
    UserSessionDataBase();
    UserSession* GetToSend( const char* login );
    int GetToSend( int index );
//    UserSession& GetSession( int index );
    UserSession* GetNewSession( int sock );
};

extern UserSessionDataBase sessionBase;

#endif /* SERVER_USER_SESSION_HPP_ */
