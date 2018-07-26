/*
 * client_communication.cpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#include <stdint.h>
#include "string.h"
#include <pthread.h>
#include "client_communication.hpp"
#include "client_session.hpp"
#include "defines.hpp"
#include "HAL_memory.hpp"
#include "HAL_UI.hpp"
#include <unistd.h>

// Function prepare the message according to the frame definition in message.hpp
ReturnCode SendMessage( const Message& msg, ClientSession& session) {
    ReturnCode retVal = RET_OK;
    timespec waitingTime;
    size_t msg_len = sizeof( MessageHeader ) + msg.m_header.m_len;
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
    if ( session.Lock( &waitingTime ) == 0 ) {
        int res;
        res = write( session.GetSocket(), buf, msg_len );
        if ( res != msg_len ) {
            LOG_E( "Sending error." );
            retVal = RET_COMMUNICATION_ERROR;
        }

        if ( session.Unlock() != 0 ) {
            LOG_E( "Unlocking mutex error." );
            retVal = RET_MUTEX_ERROR;
        }
    } else {
        LOG_E( "Unable to lock mutex during sending " );
        retVal = RET_MUTEX_ERROR;
    }
    delete buf;

    return retVal;
}

void SavePayload( Message& msg, const LoggingPayload& payload ) {
    msg.m_payload.resize( sizeof( LoggingPayload ) );
    memcpy( msg.m_payload.data(), payload.m_login, sizeof( payload.m_login ) );
    memcpy( msg.m_payload.data() + sizeof( payload.m_login ), payload.m_passHASH, sizeof( payload.m_passHASH ) );
}
