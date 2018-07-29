/*
 * HAL_memory.hpp
 *
 *  Created on: Jul 24, 2018
 *      Author: niedziu
 */

#ifndef HAL_MEMORY_HPP_
#define HAL_MEMORY_HPP_

#include <string.h>
#include <stdint.h>

template <typename ValueType>
void SetLE(void* dst, ValueType val) {
    memcpy( dst, &val, sizeof( ValueType ) );
}

template <class ValueType>
ValueType GetLE(uint8_t* value) {
    ValueType tmp;
    memcpy( &tmp, value, sizeof( ValueType ) );
    return tmp;
}

#endif /* HAL_MEMORY_HPP_ */
