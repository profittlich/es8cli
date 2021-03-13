/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <iostream>
#include <cstdio>
#include "helpers.h"

bool onlyDigits(std::string s)
{
    bool result = s.size () > 0;
    for (char c : s)
    {
        result &= (c >= '0') && (c <= '9');
    }
    return result;
}

std::vector<uint8_t> loadFile(std::string fn)
{
    std::vector<uint8_t> result;
    FILE * f = std::fopen (fn.c_str(), "rb");
    if (f)
    {
        std::fseek(f, 0, SEEK_END);
        size_t len = std::ftell(f);
        std::fseek(f, 0, SEEK_SET);
        result.resize (len);
        std::fread(result.data(), len, 1, f);
        std::fclose(f);
    }
    return result;
}

void writeFile(std::string fn, std::vector<uint8_t> data)
{
    FILE * f = std::fopen (fn.c_str(), "wb");
    if (f)
    {
        std::fwrite(data.data(), data.size(), 1, f);
        std::fclose(f);
    }
    else
    {
        std::cerr << "Could not write " << fn << std::endl;
    }
}
