/*
 * server_commands.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: niedziu
 */

#include "server_commands.hpp"
#include "server_logger.hpp"
#include "server_user.hpp"
#include <string.h>

void LoggingReq( UserSession& session, const Message& msg ) {
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
        session.SetLogin( payload_req.m_login );
        session.SetStatus( UserSessionStatus_Logged );
    } else {
        payload_ans.m_anwser[0] = LOGREQ_RET_ERROR;
        LOG_I("Invalid passes \n");
    }
    SavePayload( msg_ans, payload_ans );
    SendMessage( msg_ans, session );
}

void TextMsg( UserSession& session, const Message& msg ) {
    // TODO: Find a better way to get the login from the message
    const char* dst_login = ( const char* ) ( msg.m_payload.data() + LOGIN_MAX_SIZE );
    UserSession *session_dst = sessionBase.GetToSend( dst_login );
    if( session_dst == NULL ) {
        Message msg_ans;
        TextControlPayload payload;
        strcpy( payload.m_login, dst_login );
        payload.m_control[0] = TEXTCTRL_USERUNLOGGED;
        SavePayload( msg_ans, payload );
        msg_ans.m_header.m_len = sizeof( TextControlPayload );
        msg_ans.m_header.m_type = MSGTYPE_TEXT_CONTROL;
        LOG_I( "Dst user unlogged " << msg.m_payload.data() + LOGIN_MAX_SIZE <<" \n" );
        SendMessage( msg_ans, session );
    } else {
        LOG_I( "Main: sending msg \n" );
        SendMessage( msg, *session_dst );
    }
}
