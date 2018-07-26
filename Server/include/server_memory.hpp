/*
 * server_memory.hpp
 *
 *  Created on: Jul 26, 2018
 *      Author: niedziu
 */

#ifndef SERVER_MEMORY_HPP_
#define SERVER_MEMORY_HPP_

#include <string>

template <class type>
type GetLE(uint8_t* value) {
    type tmp;
    memcpy( &tmp, value, sizeof( type ) );
    return tmp;
}


#endif /* SERVER_MEMORY_HPP_ */
