/*
 * HAL_UI.hpp
 *
 *  Created on: 21-07-2018
 *      Author: HP
 */

#ifndef HAL_UI_HPP_
#define HAL_UI_HPP_

#include <iostream>

#define LOG_I(x) (std::cout << ( x ) )
#define LOG_E(x) (std::cerr <<  x  )

char HAL_UI_GetChar();

#endif /* HAL_UI_HPP_ */
