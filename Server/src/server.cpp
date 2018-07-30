#include <iostream>
#include <queue>
#include "server_session.hpp"
#include "defines.hpp"
#include "message.hpp"
#include "server_logger.hpp"
#include "server_communication.hpp"
#include "server_user.hpp"
#include "server_user_session.hpp"
#include <pthread.h>
#include <unistd.h>
#include <string.h>

UserSessionDataBase base;

void* HandleNewUser( void* arg) {
    int sock = *((int*)arg );
    UserSession* session = base.GetNewSession( sock );

    delete (int*)arg;

    Message msg;

    while (1) {
        if ( ReciveMessage( msg, *session ) == RET_OK ) {
            switch ( msg.m_header.m_type ) {
            case MSGTYPE_LOGGING_REQ:
                User us1;
                memcpy( &us1, msg.m_payload.data(), sizeof( User ) );
                if ( UserDataBase::Instance().VerifyUser( us1 ) == RET_OK ) {
                    Message msg_ans;
                    LoggingAnsPayload payload;
                    strcpy( payload.m_login, us1.m_login );
                    payload.m_anwser[0] = ANS_OK;
                    msg_ans.m_payload.resize( sizeof ( LoggingAnsPayload ) );
                    memcpy( msg_ans.m_payload.data(), (uint8_t*) &payload, sizeof( LoggingAnsPayload ) );
                    msg_ans.m_header.m_len = 64;
                    msg_ans.m_header.m_type = MSGTYPE_LOGGING_ANS;
                    SendMessage( msg_ans, *session );
                    LOG_I( us1.m_login << " logged!\n");
                    session->SetLogin( us1.m_login );
                    session->SetStatus( UserSessionStatus_Logged );
                } else {
                    LOG_I("Invalid passes \n");
                }
                break;
            default:
            break;
            }

        } else {
            std::cout<< "End of connection ?\n";
            session->Close();
            break;
        }
    }

//    std::cout<< "New thread user. Sock id: " << sock << "\n";
//    close(sock);
    return NULL;
}


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
//                char a;
//                std::cin >> a;
//                UserSession *session = base.GetToSend("Dawid");
//                if( session == NULL ) {
//                    LOG_E( "Main: invalid session \n" );
//                } else {
//                    LOG_I( "Main: sending msg \n" );
//                    Message msg;
//                    msg.m_header.m_len = 10;
//                    msg.m_header.m_type = 3;
//                    msg.m_payload.resize( 10, 3 );
//                    SendMessage( msg, *session );
//                }
            }
        }
    }

	return 0;
}
