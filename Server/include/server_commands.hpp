/*
 * server_commands.hpp
 *
 *  Created on: Aug 3, 2018
 *      Author: niedziu
 */

#ifndef SERVER_COMMANDS_HPP_
#define SERVER_COMMANDS_HPP_

#include "server_user_session.hpp"
#include "message.hpp"

void LoggingReq( UserSession& session, const Message& msg );
void TextMsg( UserSession& session, const Message& msg );

#endif /* SERVER_COMMANDS_HPP_ */
