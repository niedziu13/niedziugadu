/*
 * client_session.cpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#include "client_session.hpp"
#include <string.h>

ClientSession::ClientSession(){
    memset( &m_serverAddress, 0, sizeof( sockaddr_in ) );
}
