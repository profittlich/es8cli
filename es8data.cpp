/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "es8parameters.hpp"
#include "es8data.hpp"
#include "bitcoder.hpp"

void ES8Data::setData (std::vector<uint8_t> & data)
{
    if (dataValid(data))
    {
        m_data = data;
    }
    else
    {
        throw std::runtime_error("Data invalid");
    }
}

const std::vector<uint8_t> ES8Data::data()
{
    return m_data;
}

std::vector<uint8_t> & ES8Data::writeableData()
{
    return m_data;
}

void ES8Data::printVerbose()
{
    writeFileData (std::cout);
}

void ES8Data::loadValues (std::ifstream & infile)
{
    BitCodec bc (writeableData());

    std::string line;

    while (std::getline(infile, line))
    {
        std::istringstream conv (line);
        std::string key, value;
        std::getline (conv, key, ':');
        std::getline (conv, value, ':');
        std::stringstream trim;
        trim << value;
        value.clear();
        trim >> value;
        std::string id;
        unsigned index = 1;
        try
        {
            id = g_fields.at(key).id();
        }
        catch (const std::out_of_range & e)
        {
            id = g_fields.at(key.substr(0, key.find_last_of('_')+1)).id();
	    std::stringstream conv;
            conv << key.substr(key.find_last_of('_')+1, key.length());
            conv >> index;
        }

        unsigned intValue = g_fields.at(id).value (value);
        
        bc.setValue (g_fields.at(id).bitOffset(index-1), g_fields.at(id).bitLength(), intValue);
    }
}

