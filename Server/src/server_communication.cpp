/*
 * server_communication.cpp
 *
 *  Created on: Jul 26, 2018
 *      Author: niedziu
 */

#include "server_communication.hpp"
#include <unistd.h>
#include <string.h>
#include "server_memory.hpp"
#include "server_logger.hpp"
#include <pthread.h>

ReturnCode ReciveMessage( Message& msg, UserSession& session ) {
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
        LOG_E( "Connection closed. \n" );
        retVal = RET_COMMUNICATION_ERROR;
    } else {
        if ( static_cast<uint16_t>( res ) != msg_header_len ) {
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
            if ( res != msg.m_header.m_len ) {
              // TODO: Read the rest of the payload
            }
            LOG_I( "Message recived. Size: " << res << " + " << res2 << "\n" );
            msg.m_payload.resize( msg.m_header.m_len );
            memcpy( msg.m_payload.data(), buf_payload, msg.m_header.m_len );
        }
    }

    return retVal;
}
