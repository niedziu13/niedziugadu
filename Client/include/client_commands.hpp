/*
 * client_commands.hpp
 *
 *  Created on: Jul 30, 2018
 *      Author: niedziu
 */

#ifndef CLIENT_COMMANDS_HPP_
#define CLIENT_COMMANDS_HPP_

#include "message.hpp"
#include "client_session.hpp"

#endif /* CLIENT_COMMANDS_HPP_ */

ReturnCode LoggingAns( ClientSession& session, const Message& msg );

ReturnCode TextMsgRec( ClientSession& session, const Message& msg );

ReturnCode TextMsgSend( ClientSession& session );

ReturnCode TextCtrlRec( const Message& msg );
