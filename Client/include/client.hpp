/*
 * client.hpp
 *
 *  Created on: 22-07-2018
 *      Author: HP
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <assert.h>
#include <iostream>
#include "defines.h"

/**
 * 1. A program name
 * 2. An address IP of a server
 */
#define NUMBER_OF_ARGS 2

ReturnCode ParseArgs(int argc, char** argv);

#endif /* CLIENT_HPP_ */