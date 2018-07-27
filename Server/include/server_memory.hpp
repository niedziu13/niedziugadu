/*
 * server_memory.hpp
 *
 *  Created on: Jul 26, 2018
 *      Author: niedziu
 */

#ifndef SERVER_MEMORY_HPP_
#define SERVER_MEMORY_HPP_

#include <string>

template <class ValueType>
ValueType GetLE(uint8_t* value) {
    ValueType tmp;
    memcpy( &tmp, value, sizeof( ValueType ) );
    return tmp;
}

template <class ValueType>
void SetLE(void* dst, ValueType val) {
    memcpy( dst, &val, sizeof( ValueType ) );
}


#endif /* SERVER_MEMORY_HPP_ */
