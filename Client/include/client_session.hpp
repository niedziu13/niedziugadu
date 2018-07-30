/*
 * client_session.hpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#ifndef CLIENT_SESSION_HPP_
#define CLIENT_SESSION_HPP_

#include <sys/socket.h>
#include <arpa/inet.h>
#include "defines.hpp"
#include "message.hpp"
#include <pthread.h>

enum ClientSessionStatus {
    ClientSessionStatus_Disconnected,
    ClientSessionStatus_Connceted,
    ClientSessionStatus_LogingPending,
    ClientSessionStatus_Logged
};

class ClientSession {
    struct sockaddr_in m_serverAddress;
    int m_serverSocket;
    ClientSessionStatus m_status;
    char m_login[LOGIN_MAX_SIZE];
    pthread_mutex_t m_sendMutex; // Mutex of send operation
    pthread_mutex_t m_sessionMutex; // Mutex of session status
public:
    ClientSession();
    ~ClientSession();
    int UpdateAddress( const char* address );
    int GetSocket() const;
    ReturnCode Connect();
    ReturnCode Disconnect();
    ReturnCode Login();
    ReturnCode Close();
    int LockSend();
    int LockSend( const timespec* time );
    int UnlockSend();
    int LockSession();
    int UnlockSession();
    const char* GetLogin() const;
    ClientSessionStatus GetStatus() const;
    void SetStatus( ClientSessionStatus status );
};

#endif /* CLIENT_SESSION_HPP_ */
