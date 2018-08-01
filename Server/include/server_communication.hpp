/*
 * server_communication.hpp
 *
 *  Created on: Jul 26, 2018
 *      Author: niedziu
 */

#ifndef SERVER_COMMUNICATION_HPP_
#define SERVER_COMMUNICATION_HPP_

#include "defines.hpp"
#include "message.hpp"
#include "server_user_session.hpp"

#define WAITING_SEND_TIME_S 5
#define WAITING_SEND_TIME_NS 0

ReturnCode ReciveMessage( Message& msg, UserSession& session );

ReturnCode SendMessage( const Message& msg, UserSession& session );

void SavePayload( Message& msg, const LoggingAnsPayload& payload );
void SavePayload( Message& msg, const TextControlPayload& payload );
void LoadPayload( const Message& msg, LoggingReqPayload& payload );

#endif /* SERVER_COMMUNICATION_HPP_ */
