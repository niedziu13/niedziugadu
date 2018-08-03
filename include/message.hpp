/*
 * message.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <stdint.h>
#include <time.h>
#include <vector>
#include "defines.hpp"
#define PAYLOAD_SIZE 256

typedef uint8_t msgType_t;
typedef uint16_t msgLen_t;

#define MSGTYPE_TEXT_MSG ( (msgType_t) 0x01 )
#define MSGTYPE_LOGGING_REQ ( (msgType_t) 0x02 )
#define MSGTYPE_LOGGING_ANS ( (msgType_t) 0x03 )
#define MSGTYPE_TEXT_CONTROL ( (msgType_t) 0x04 )

#define LOGREQ_RET_OK ( (uint8_t) 0x00 )
#define LOGREQ_RET_ERROR ( (uint8_t) 0x01 )

#define TEXTCTRL_USERUNLOGGED ( (uint8_t) 0x01 )

#define LOGIN_MAX_SIZE 32U // Including \0 in the string
#define HASH_SIZE 32U


class Session {
public:
    virtual int GetSocket() const = 0;
    virtual int LockSend( const timespec* time ) = 0;
    virtual int UnlockSend() = 0;
};

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
  Logging request payload in memory
  BYES  0123456701234567012345670123456701234567012345670123456701234567
        |m_login                       | m_passHASH                    |
*/
struct LoggingReqPayload {
    char m_login[LOGIN_MAX_SIZE];
    uint8_t m_passHASH[HASH_SIZE];
};

/*
  Logging answer payload in memory
  BYES  0123456701234567012345670123456701234567012345670123456701234567
        |m_login                       | m_anwser                      |
*/
struct LoggingAnsPayload {
    char m_login[LOGIN_MAX_SIZE];
    uint8_t m_anwser[HASH_SIZE]; // Only the first byte matters
};

/*
  Text message payload in memory
  BYES  0123456701234567012345670123456701234567012345670123456701234567
        |m_loginSrc                     | m_loginDst                   |
        |text ...

*/
struct TextMsgPayload {
    char m_loginSrc[LOGIN_MAX_SIZE];
    char m_loginDst[LOGIN_MAX_SIZE];
    std::vector<char> m_text;
};

/*
  Text control payload in memory
  BYES  0123456701234567012345670123456701234567012345670123456701234567
        |m_login                        | m_control                    |

*/
struct TextControlPayload {
    char m_login[LOGIN_MAX_SIZE];
    uint8_t m_control[LOGIN_MAX_SIZE];
};


ReturnCode SendMessage( const Message& msg, Session& session );
ReturnCode ReciveMessage( Message& msg, Session& session );

void SavePayload( Message& msg, const LoggingReqPayload& payload );
void SavePayload( Message& msg, const LoggingAnsPayload& payload );
void SavePayload( Message& msg, const TextMsgPayload& payload );
void SavePayload( Message& msg, const TextControlPayload& payload );

void LoadPayload( const Message& msg, LoggingAnsPayload& payload );
void LoadPayload( const Message& msg, LoggingReqPayload& payload );
void LoadPayload( const Message& msg, TextMsgPayload& payload );
void LoadPayload( const Message& msg, TextControlPayload& payload );

#endif /* MESSAGE_HPP_ */
