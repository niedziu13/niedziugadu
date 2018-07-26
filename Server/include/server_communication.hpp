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

ReturnCode ReciveMessage( Message& msg, UserSession& session );

#endif /* SERVER_COMMUNICATION_HPP_ */
