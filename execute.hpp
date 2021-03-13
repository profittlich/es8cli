/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>
#include "commands.hpp"
#include "commandline.hpp"

///@todo document and OOP

bool validateProgram(CmdLineParameters config);
void runProgram(CmdLineParameters config);
void executeCommand (const Command & cmd, std::vector<uint8_t> & data, const CmdLineParameters & config);
