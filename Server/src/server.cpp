#include <iostream>
#include <queue>
#include "server_session.hpp"
#include "defines.hpp"
#include "message.hpp"
#include "server.hpp"
#include "server_logger.hpp"
#include "server_communication.hpp"
#include "server_commands.hpp"
#include "server_user_session.hpp"
#include <pthread.h>
#include <unistd.h>
#include <string.h>


int main() {
    ServerSession session;
    std::cout << "Siema tutaj serwer" << std::endl;
    int *userSock;

    if ( session.SetupAndListen() == RET_OK ) {
        std::cout << "Server listing" << std::endl;
        while(1) {
            userSock = new int;
            *userSock = session.Accept();
            if ( *userSock < 0 ) {
                LOG_E( "Accept error. \n" );
                delete userSock;
            } else {
                pthread_t thread;
                LOG_I( "A new user connected. \n" );
                pthread_create(&thread, NULL, HandleNewUser, (void*)userSock);
            }
        }
    }

	return 0;
}

void* HandleNewUser( void* arg) {
    int sock = *((int*)arg );
    UserSession* session = sessionBase.GetNewSession( sock );

    delete (int*)arg;

    Message msg;

    while (1) {
        if ( ReciveMessage( msg, *session ) == RET_OK ) {
            switch ( msg.m_header.m_type ) {
            case MSGTYPE_LOGGING_REQ:
            {
                LoggingReq( *session, msg );
                break;
            }
            case MSGTYPE_TEXT_MSG:
            {
                TextMsg( *session, msg );
                break;
            }
            default:
            {
                break;
            }
            }
        } else {
            std::cout<< "End of connection ?\n";
            session->Close();
            break;
        }
    }

    return NULL;
}
