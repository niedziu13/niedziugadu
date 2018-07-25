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

#endif /* HAL_MEMORY_HPP_ */
