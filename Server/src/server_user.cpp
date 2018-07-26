/*
 * server_user.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: niedziu
 */

#include "server_user.hpp"
#include "string.h"

UserDataBase::UserDataBase() {
    User usr;
    strcpy( usr.m_login, "Dawid" );
    memset( usr.m_passwordHASH, 1, HASH_SIZE );
    m_base.push_back( usr );
    strcpy( usr.m_login, "Adam" );
    m_base.push_back( usr );
}

UserDataBase& UserDataBase::Instance() {
    static UserDataBase base;
    return base;
}

ReturnCode UserDataBase::VerifyUser( const User& usr ) {
    ReturnCode retVal = RET_INVALID_CREDENTIALS;
    for( auto i = m_base.begin(); i != m_base.end(); ++i) {
        if( strcmp( i->m_login, usr.m_login ) == 0 ) {
            if( memcmp( i->m_passwordHASH, usr.m_passwordHASH, HASH_SIZE ) == 0 ) {
                retVal = RET_OK;
            }
            break;
        }
    }
    return retVal;
}
