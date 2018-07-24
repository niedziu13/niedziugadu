/*
 * HAL_UI.hpp
 *
 *  Created on: 21-07-2018
 *      Author: HP
 */

#ifndef HAL_UI_HPP_
#define HAL_UI_HPP_

#include <iostream>
#include "defines.hpp"

#define LOG_I(x) (std::cout << ( x ) )
#define LOG_E(x) (std::cerr <<  x  )

char HAL_UI_GetChar();
ReturnCode HAL_UI_GetLogin( char* login );

#endif /* HAL_UI_HPP_ */
