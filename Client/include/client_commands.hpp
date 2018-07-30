/*
 * client_commands.hpp
 *
 *  Created on: Jul 30, 2018
 *      Author: niedziu
 */

#ifndef CLIENT_COMMANDS_HPP_
#define CLIENT_COMMANDS_HPP_

#include <client_communication.hpp>


#endif /* CLIENT_COMMANDS_HPP_ */

ReturnCode LoggingAns( ClientSession& session, const Message& msg );
ReturnCode TextRec( ClientSession& session, const Message& msg );
ReturnCode TextSend( ClientSession& session );

