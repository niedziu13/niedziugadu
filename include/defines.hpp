/*
 * defines.hpp
 *
 *  Created on: Jul 20, 2018
 *      Author: niedziu
 */

#ifndef DEFINES_HPP_
#define DEFINES_HPP_

#include <assert.h>

enum ReturnCode {
    RET_INVALID,
    RET_OK,
    RET_UNEXISTING_OPTION,
    RET_EXISTING_OPTION,
    RET_INVALID_OPTION_FOUND,
    RET_INVALID_NUMBER_OR_ARGS,
    RET_INVALID_ADDRESS
};

#endif /* DEFINES_HPP_ */
