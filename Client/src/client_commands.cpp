/*
 * client_commands.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: niedziu
 */

#include "client_commands.hpp"
#include "HAL_UI.hpp"
#include <string.h>
#include <pthread.h>

ReturnCode LoggingAns( ClientSession& session, const Message& msg ) {
    ReturnCode retVal = RET_OK;
    LoggingAnsPayload payload;

    LoadPayload( msg, payload );
    if ( session.LockSession() != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( session.GetStatus() != ClientSessionStatus_LogingPending ) {
            LOG_E( "No logging pending.\n" );
            retVal = RET_SESSION_STATUS_ERROR;
        } else if ( strcmp( session.GetLogin(), payload.m_login ) != 0 ){
            LOG_E( "Logging answer for different user.\n" );
            retVal = RET_SESSION_STATUS_ERROR;
        } else if ( payload.m_anwser[0] != LOGREQ_RET_OK ) {
            LOG_E( "Unable to login as: " << payload.m_login << "\n" );
            retVal = RET_INVALID_CREDENTIALS;
        } else {
            LOG_I( "Logged as: " << payload.m_login << "\n" );
            session.SetStatus( ClientSessionStatus_Logged );
        }
        if ( session.UnlockSession() != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
    }
    return retVal;
}

ReturnCode TextMsgRec( ClientSession& session, const Message& msg ) {
    ReturnCode retVal = RET_OK;
    TextMsgPayload payload;

    // TODO: Not coping all text to a new memory
    LoadPayload( msg, payload );
    if ( strcmp( payload.m_loginDst, session.GetLogin() ) != 0 ) {
        LOG_E( "Received a message to somebody else\n" );
        retVal = RET_INVALID_LOGIN;
    } else {
        LOG_I( payload.m_loginSrc << ":\n" ); // TODO: Change presenting messages
        LOG_I( payload.m_text.data() << "\n");
    }
    return retVal;
}

ReturnCode TextMsgSend( ClientSession& session ) {
    ReturnCode retVal = RET_OK;

    if ( session.LockSession() != 0 ) {
        retVal = RET_MUTEX_ERROR;
    } else {
        if ( session.GetStatus() != ClientSessionStatus_Logged ) {
            LOG_E( "You are not logged.\n" );
            retVal = RET_SESSION_STATUS_ERROR;
        }
        if ( session.UnlockSession() != 0 ) {
            retVal = RET_MUTEX_ERROR;
        }
        if ( retVal == RET_OK ) {
            // TODO: Create a new window
            TextMsgPayload payload;
            if ( HAL_UI_GetLogin( payload.m_loginDst ) != RET_OK ) {
                retVal = RET_INVALID_LOGIN;
            } else {
                Message msg;
                unsigned sizePayload;
                std::string buf;

                LOG_I( "Text: \n" );
                std::getline( std::cin, buf );
                // Size returns a number of chars without '\0'
                sizePayload = sizeof( payload.m_loginDst ) + sizeof( payload.m_loginDst ) + buf.size() + 1;
                // Prepare header
                msg.m_header.m_len = sizePayload;
                msg.m_header.m_type = MSGTYPE_TEXT_MSG;
                // Prepare payload
                payload.m_text.resize( buf.size() + 1 );
                buf.copy( payload.m_text.data(), buf.size() );
                payload.m_text[ buf.size() ] = '\0';
                strcpy( payload.m_loginSrc, session.GetLogin() );
                SavePayload( msg, payload );
                retVal = SendMessage( msg, session );
            }
        }
    }
    return retVal;
}

ReturnCode TextCtrlRec( const Message& msg ) {
    ReturnCode retVal = RET_OK;
    TextControlPayload payload;

    LoadPayload( msg, payload );
    switch( payload.m_control[0] ) {
    case TEXTCTRL_USERUNLOGGED:
        LOG_I( "User " << payload.m_login << " is not logged.\n" );
        break;
    default:
        LOG_E( "Unrecognized TextContralValue.\n" );
        break;
    }
    return retVal;
}

