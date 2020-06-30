// MeyaS for Meya Socket or Meya·S·Ephemeral, both of which are good names. (Really?)
// Author: AsterNighT https://github.com/AsterNighT
// Github: https://github.com/AsterNighT/MeyaS
// Wish I would not write trash code again in this project.
//
// The only file should be included to use the lib
#pragma once
#include "common.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

namespace MeyaS {
    // Initialize Winsock
    bool initialize(WORD version = MAKEWORD(2, 2));
}
