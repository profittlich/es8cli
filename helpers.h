/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>
#include <string>

///@todo document

bool onlyDigits(std::string s);
std::vector<uint8_t> loadFile(std::string fn);
void writeFile(std::string fn, std::vector<uint8_t> data);
