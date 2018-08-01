/*
 * client_communication.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef CLIENT_COMMUNICATION_HPP_
#define CLIENT_COMMUNICATION_HPP_

#define WAITING_SEND_TIME_S 5
#define WAITING_SEND_TIME_NS 0

#include "client_session.hpp"
#include "message.hpp"
#include "defines.hpp"

ReturnCode SendMessage( const Message& msg, ClientSession& session);
ReturnCode ReciveMessage( Message& msg, ClientSession& session );

void SavePayload( Message& msg, const LoggingReqPayload& payload );
void SavePayload( Message& msg, const TextMsgPayload& payload );
void LoadPayload( const Message& msg, LoggingAnsPayload& payload );
void LoadPayload( const Message& msg, TextMsgPayload& payload );
void LoadPayload( const Message& msg, TextControlPayload& payload );

#endif /* CLIENT_COMMUNICATION_HPP_ */
