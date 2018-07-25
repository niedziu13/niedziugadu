/*
 * message.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <stdint.h>
#include <vector>
#include "defines.hpp"
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
    msgType_t m_pad; // padding
};

/*
  Depending on arch the structure in memory can look different.
  Message in memory should look like (but the size has to be 4 bytes):
  BITS  01234567012345670123456701234567012...
        |m_len(LE)     |m_type |m_pad  | payload ...

 */

struct Message {
    MessageHeader m_header;
    std::vector<uint8_t> m_payload;
};


/*
  Logging payload in memory
  BYES  0123456701234567012345670123456701234567012345670123456701234567
        |m_login                       | m_passHASH                    |
*/

struct LoggingPayload {
    char m_login[LOGIN_MAX_SIZE];
    uint8_t m_passHASH[HASH_SIZE];
}; // __attribute__((packed));


#endif /* MESSAGE_HPP_ */
