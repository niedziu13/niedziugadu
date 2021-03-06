/*
 * message.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: niedziu
 */

#include "message.hpp"
#include <unistd.h>
#include <string.h>

#ifdef client
#include "HAL_UI.hpp"
#include "HAL_memory.hpp"
#include "client_communication.hpp"
#endif
#ifdef server
#include "server_logger.hpp"
#include "server_memory.hpp"
#include "server_communication.hpp"
#endif

ReturnCode ReciveMessage( Message& msg, Session& session ) {
    ReturnCode retVal = RET_OK;
    int res;
    int res2;
    size_t msg_header_len = sizeof( MessageHeader );
    uint8_t *buf_header = new uint8_t[ msg_header_len ];

    res = read( session.GetSocket(), buf_header, msg_header_len );
    if ( res < 0 ) {
        LOG_E( "Connection error. \n" );
        retVal = RET_COMMUNICATION_ERROR;
    } else if ( res == 0 ) {
        LOG_E( "Read = 0. Connection closed. \n" );
        retVal = RET_COMMUNICATION_ERROR;
    } else {
        if ( static_cast<uint16_t>( res ) != msg_header_len ) {
          LOG_I( "Read not complete header. \n" );
          // TODO: Read the rest of the header
        }
        msg.m_header.m_len = GetLE<msgLen_t>( buf_header );
        msg.m_header.m_type = GetLE<msgType_t>( buf_header + sizeof( msgLen_t ));
        uint8_t *buf_payload = new uint8_t[ msg.m_header.m_len ];
        res2 = read( session.GetSocket(), buf_payload, msg.m_header.m_len );
        if ( res2 < 0 ) {
            LOG_E( "Connection error. \n" );
            retVal = RET_COMMUNICATION_ERROR;
        } else if ( res2 == 0 ) {
            LOG_E( "Connection closed. \n" );
            retVal = RET_COMMUNICATION_ERROR;
        } else {
            if ( res2 != msg.m_header.m_len ) {
                LOG_I( "Read not complete payload. \n" );
              // TODO: Read the rest of the payload
            }
            LOG_I( "Message recived. Size: " << res << " + " << res2 << "\n" );
            msg.m_payload.resize( msg.m_header.m_len );
            memcpy( msg.m_payload.data(), buf_payload, msg.m_header.m_len );
        }
        delete buf_payload;
    }

    delete buf_header;

    return retVal;
}

// Function prepare the message according to the frame definition in message.hpp
ReturnCode SendMessage( const Message& msg, Session& session) {
    ReturnCode retVal = RET_OK;
    timespec waitingTime;
    int msg_len = sizeof( MessageHeader ) + msg.m_header.m_len;
    uint8_t *buf = new uint8_t[ msg_len ];
    // Prepare buffor
    // Msg header
    SetLE<msgLen_t>( buf, msg.m_header.m_len );
    SetLE<msgType_t>( buf + sizeof( msgLen_t ), msg.m_header.m_type );
    SetLE<msgType_t>( buf + sizeof( msgLen_t ) + sizeof( msgType_t ), msg.m_header.m_pad );

    // Payload
    memcpy( buf + sizeof( MessageHeader ), msg.m_payload.data(), msg.m_header.m_len );

    waitingTime.tv_sec = WAITING_SEND_TIME_S;
    waitingTime.tv_nsec = WAITING_SEND_TIME_NS;

    // Mutex blocked
    if ( session.LockSend( &waitingTime ) != 0 ) {
        LOG_E( "Unable to lock mutex during sending " );
        retVal = RET_MUTEX_ERROR;
    } else {
        int res;
        res = write( session.GetSocket(), buf, msg_len );
        if ( res != msg_len ) {
            LOG_E( "Sending error." );
            retVal = RET_COMMUNICATION_ERROR;
        }

        if ( session.UnlockSend() != 0 ) {
            LOG_E( "Unlocking mutex error." );
            retVal = RET_MUTEX_ERROR;
        }
    }

    delete buf;

    return retVal;
}

void SavePayload( Message& msg, const LoggingReqPayload& payload ) {
    msg.m_payload.resize( sizeof( LoggingReqPayload ) );
    memcpy( msg.m_payload.data(), payload.m_login, sizeof( payload.m_login ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_login ), payload.m_passHASH, sizeof( payload.m_passHASH ) );
}

void SavePayload( Message& msg, const LoggingAnsPayload& payload ) {
    msg.m_payload.resize( sizeof( LoggingAnsPayload ) );
    memcpy( msg.m_payload.data(), payload.m_login, sizeof( payload.m_login ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_login ), payload.m_anwser, sizeof( payload.m_anwser ) );
}

void SavePayload( Message& msg, const TextMsgPayload& payload ) {
    int textSize = msg.m_header.m_len - sizeof( payload.m_loginSrc ) - sizeof( payload.m_loginDst );
    assert( textSize >= 1 );
    msg.m_payload.resize( msg.m_header.m_len );
    memcpy( msg.m_payload.data(), payload.m_loginSrc, sizeof( payload.m_loginSrc ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_loginSrc ), payload.m_loginDst, sizeof( payload.m_loginDst ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_loginSrc ) + sizeof( payload.m_loginDst ), payload.m_text.data(), textSize );
}

void SavePayload( Message& msg, const TextControlPayload& payload ) {
    msg.m_payload.resize( sizeof( TextControlPayload ) );
    memcpy( msg.m_payload.data(), payload.m_login, sizeof( payload.m_login ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_login ), payload.m_control, sizeof( payload.m_control ) );
}

void LoadPayload( const Message& msg, LoggingAnsPayload& payload ) {
    memcpy( payload.m_login, msg.m_payload.data(), sizeof( payload.m_login ) );
    memcpy( payload.m_anwser , msg.m_payload.data() + sizeof( payload.m_login ), sizeof( payload.m_anwser) );
}

void LoadPayload( const Message& msg, LoggingReqPayload& payload ) {
    memcpy( payload.m_login, msg.m_payload.data(), sizeof( payload.m_login ) );
    memcpy( payload.m_passHASH , msg.m_payload.data() + sizeof( payload.m_passHASH ), sizeof( payload.m_passHASH) );
}

void LoadPayload( const Message& msg, TextControlPayload& payload ) {
    memcpy( payload.m_login, msg.m_payload.data(), sizeof( payload.m_login ) );
    memcpy( payload.m_control , msg.m_payload.data() + sizeof( payload.m_login ), sizeof( payload.m_control) );
}

void LoadPayload( const Message& msg, TextMsgPayload& payload ) {
    int textSize = msg.m_header.m_len - sizeof( payload.m_loginSrc ) - sizeof( payload.m_loginDst );
    assert( textSize >= 1 );
    memcpy( payload.m_loginSrc, msg.m_payload.data(), sizeof( payload.m_loginSrc ) );
    memcpy( payload.m_loginDst, msg.m_payload.data() + sizeof( payload.m_loginSrc ), sizeof( payload.m_loginDst ) );
    payload.m_text.resize( textSize );
    memcpy( payload.m_text.data() , msg.m_payload.data() + sizeof( payload.m_loginSrc ) + sizeof( payload.m_loginDst ), textSize );
}
