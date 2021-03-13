/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "es8parameters.hpp"
#include "globals.hpp"
#include "bitcoder.hpp"

Globals::Globals ()
{
    writeableData().resize (8 * 250);
}

void Globals::print()
{
    printVerbose();
}

void Globals::writeFileData(std::ostream & s)
{
    auto dataCopy = data();
    BitDecoder bc (dataCopy);

    for (auto it = g_fields.begin(); it != g_fields.end(); ++it)
    {
        if (it->second.type() == Field::Globals)
        {
            for (auto i = 0; i < it->second.numFields(); ++i)
            {
		if (it->second.numFields() == 1)
                {
                    s << it->second.id() << ": " << it->second.value (bc.getValue(it->second.bitOffset(i), it->second.bitLength())) << std::endl;
                }
                else
                {
                    s << it->second.id() << "" << (i+1) << ": " << it->second.value (bc.getValue(it->second.bitOffset(i), it->second.bitLength()));
                    if (it->second.id() == "ID_PATCH_NAME_")
                    {
                        s << " '" << char(bc.getValue(it->second.bitOffset(i), it->second.bitLength())) << "'";
                    }
                    s << std::endl;
                }
            }
        }
    }
}

void Globals::load(std::string filename)
{
    std::ifstream infile (filename);
    
    std::string line;
    std::getline(infile, line);
    if (line != "ES8cli globals file format 1")
    {
        std::cerr << "wrong file format <" << line << ">" << std::endl;
        throw std::runtime_error ("Wrong globals file format");
    }

    loadValues (infile);
}

void Globals::save(std::string filename)
{
    std::ofstream backupFile;
    backupFile.open ("globals.es8");
    backupFile << "ES8cli globals file format 1" << std::endl;
    writeFileData(backupFile);
    backupFile.close();
}

bool Globals::dataValid(std::vector<uint8_t> & data)
{
    return (data.size () == 8 * 250);
}

