/*
 * HAL_UI.cpp
 *
 *  Created on: 21-07-2018
 *      Author: HP
 */

#include "HAL_UI.hpp"
#include "message.hpp"
#include <string.h>
#include <string>

char HAL_UI_GetChar(){
    char key;
    std::cin >> key;
    std::cin.clear();
    std::cin.ignore( 1000, '\n' );
    return key;
}

ReturnCode HAL_UI_GetLogin( char* login ) {
    ReturnCode retVal = RET_OK;
    std::string buf;
    LOG_I( "Login: \n" );
    std::getline( std::cin, buf );
    // Size returns a number of chars without '\0'
    if ( buf.size() > LOGIN_MAX_SIZE - 1) {
        LOG_I( "Too long login. \n" );
        retVal = RET_INVALID_LOGIN;
    } else {
        buf.copy( login, buf.size() );
        login[ buf.size() ] = '\0';
    }
    return retVal;
}
