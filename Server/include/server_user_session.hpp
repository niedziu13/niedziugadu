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

enum UserSessionStatus {
    UserSessionStatus_Logged,
    UserSessionStatus_Connected,
    UserSessionStatus_Invalid
};

class UserSession {
    char m_login[LOGIN_MAX_SIZE];
    UserSessionStatus m_status;
    pthread_mutex_t m_writeMutex;
    pthread_mutex_t m_readMutex;
    std::queue<Message> m_writeQueue;
    std::queue<Message> m_readQueue;
    int m_sock;
public:
    UserSession( int sock );
    ~UserSession();
    ReturnCode GetMessage( Message& msg );
    ReturnCode SendMessage( const Message& msg );
    UserSessionStatus GetStatus() const;
    const char* GetLogin() const;
    int GetSocket() const;
};

class UserSessionDataBase {
    std::vector<UserSession> m_base;
    int FindUserIndex( const char* login ) const;
public:
    UserSessionStatus GetUserStatus( const char* login ) const;
};


#endif /* SERVER_USER_SESSION_HPP_ */
