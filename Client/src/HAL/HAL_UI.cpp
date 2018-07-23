/*
 * HAL_UI.cpp
 *
 *  Created on: 21-07-2018
 *      Author: HP
 */

#include "HAL_UI.hpp"

char HAL_UI_GetChar(){
    char key;
    std::cin >> key;
    std::cin.clear();
    std::cin.ignore( 1000, '\n' );
    return key;
}
