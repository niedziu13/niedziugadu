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
            {
                Message msg_ans;
                LoggingReqPayload payload_req;
                LoggingAnsPayload payload_ans;

                LoadPayload( msg, payload_req );

                msg_ans.m_header.m_len = sizeof( LoggingAnsPayload );
                msg_ans.m_header.m_type = MSGTYPE_LOGGING_ANS;
                strcpy( payload_ans.m_login, payload_req.m_login );

                if ( UserDataBase::Instance().VerifyUser( payload_req ) == RET_OK ) {
                    payload_ans.m_anwser[0] = LOGREQ_RET_OK;
                    LOG_I( payload_req.m_login << " logged!\n");
                    session->SetLogin( payload_req.m_login );
                    session->SetStatus( UserSessionStatus_Logged );
                } else {
                    payload_ans.m_anwser[0] = LOGREQ_RET_ERROR;
                    LOG_I("Invalid passes \n");
                }
                SavePayload( msg_ans, payload_ans );
                SendMessage( msg_ans, *session );
                break;
            }
            case MSGTYPE_TEXT_MSG:
            {
                // TODO: Find a better way to get the login from the message
                const char* dst_login = ( const char* ) ( msg.m_payload.data() + LOGIN_MAX_SIZE );
                UserSession *session_dst = base.GetToSend( dst_login );
                if( session_dst == NULL ) {
                    Message msg_ans;
                    TextControlPayload payload;
                    strcpy( payload.m_login, dst_login );
                    payload.m_control[0] = TEXTCTRL_USERUNLOGGED;
                    SavePayload( msg_ans, payload );
                    msg_ans.m_header.m_len = sizeof( TextControlPayload );
                    msg_ans.m_header.m_type = MSGTYPE_TEXT_CONTROL;
                    LOG_I( "Dst user unlogged " << msg.m_payload.data() + LOGIN_MAX_SIZE <<" \n" );
                    SendMessage( msg_ans, *session );
                } else {
                    LOG_I( "Main: sending msg \n" );
                    SendMessage( msg, *session_dst );
                }

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
