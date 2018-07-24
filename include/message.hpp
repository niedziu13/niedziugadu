/*
 * message.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <stdint.h>
#include "defines.h"

#define PAYLOAD_SIZE 256

typedef uint8_t msgType_t;
typedef uint16_t msgLen_t;

#define MSGTYPE_TEXT (msgType_t) 0x01
#define MSGTYPE_LOGGING (msgType_t) 0x02
#define LOGIN_MAX_SIZE 32U // Including \0 in the string
#define HASH_SIZE 32U

// Every variable is written in LittleEndian

struct MessageHeader {
    msgLen_t m_len; // Length of message not including header
    msgType_t m_type; // Message type
    msgType_t pad; // padding
};

struct Message {
    MessageHeader m_header;
    uint8_t* m_payload;
};

struct LoggingPayload {
    char login[LOGIN_MAX_SIZE];
    uint8_t passHASH[HASH_SIZE];
}; // __attribute__((packed));

//RET_CODE

#endif /* MESSAGE_HPP_ */
