/*
 * server_user.hpp
 *
 *  Created on: Jul 25, 2018
 *      Author: niedziu
 */

#ifndef SERVER_USER_HPP_
#define SERVER_USER_HPP_

#include "defines.hpp"
#include "message.hpp"
#include <vector>

struct User {
public:
    char m_login[LOGIN_MAX_SIZE];
    uint8_t m_passwordHASH[HASH_SIZE];
};

class UserDataBase {
    std::vector<User> m_base;
public:
    UserDataBase();
    static UserDataBase& Instance();
    ReturnCode VerifyUser( const LoggingReqPayload& payload );
};


#endif /* SERVER_USER_HPP_ */

