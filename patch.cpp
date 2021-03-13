/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "es8parameters.hpp"
#include "patch.hpp"
#include "bitcoder.hpp"

Patch::Patch ()
{
    writeableData().resize (250);
}

void Patch::setName(std::string name)
{
    if (name.size() > 32)
    {
        throw std::runtime_error ("Name too long. 32 characters max.");
    }

    BitCodec bc (writeableData());

    Field curField = g_fields.at("ID_PATCH_NAME_");
    for (auto i = 0; i < curField.numFields(); ++i)
    {
        bc.setValue(curField.bitOffset(i), curField.bitLength(), ' ');
    }

    for (auto i = 0; i < curField.numFields() && i < name.size(); ++i)
    {
        if (name[i] < curField.min() || name[i] > curField.max ())
        {
            throw std::runtime_error ("Invalid character in name.");
        }
        bc.setValue(curField.bitOffset(i), curField.bitLength(), name[i]);
    }
}

void Patch::setPatchMidiChannel(size_t index, Command::Parameter channelParam)
{
    BitCodec bc (writeableData());

    Field curField = g_fields.at ("ID_PATCH_MIDI_TX_CH_");

    unsigned channel = 0;
    if (channelParam.isNumber())
    {
        channel = channelParam.num();
    }
    else
    {
        channel = curField.value(channelParam.str());
    }

    if (index > curField.numFields() || index < 1)
    {
        throw std::runtime_error ("Invalid MIDI setting index.");
    }
    if (channel < curField.min() || channel > curField.max ())
    {
        throw std::runtime_error ("Invalid MIDI channel.");
    }
    
    bc.setValue(curField.bitOffset(index-1), curField.bitLength(), channel);
}

void Patch::setPatchMidiPC(size_t index, Command::Parameter pcParam)
{
    BitCodec bc (writeableData());

    Field curField = g_fields.at ("ID_PATCH_MIDI_PC_");

    unsigned pc = 0;
    if (pcParam.isNumber())
    {
        pc = pcParam.num();
    }
    else
    {
        pc = curField.value(pcParam.str());
    }

    if (index > curField.numFields() || index < 1)
    {
        throw std::runtime_error ("Invalid MIDI setting index.");
    }
    if (pc < curField.min() || pc > curField.max ())
    {
        throw std::runtime_error ("Invalid MIDI program.");
    }
    
    bc.setValue(curField.bitOffset(index-1), curField.bitLength(), pc);
}

void Patch::setPatchMidiCC(size_t index, size_t ccindex, Command::Parameter ccParam, unsigned val)
{
    Field ccField = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_");
    Field valField = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_VAL_");

    unsigned cc = 0;
    if (ccParam.isNumber())
    {
        cc = ccParam.num() + 1;
    }
    else
    {
        cc = ccField.value(ccParam.str());
    }

    BitCodec bc (writeableData());

    switch (ccindex)
    {
        case 1:
            ccField = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_");
            valField = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_VAL_");
            break;
        case 2:
            ccField = g_fields.at ("ID_PATCH_MIDI_CTL2_CC_");
            valField = g_fields.at ("ID_PATCH_MIDI_CTL2_CC_VAL_");
            break;
        default:
            throw std::runtime_error ("Only CC settings 1 or 2 possible");
    }
    
    if (index > ccField.numFields() || index < 1)
    {
        throw std::runtime_error ("Invalid MIDI setting index.");
    }
    if (ccindex < 1 || ccindex > 2)
    {
        throw std::runtime_error ("Invalid MIDI CC setting index.");
    }
    if (cc < ccField.min() || cc > ccField.max ())
    {
        throw std::runtime_error ("Invalid MIDI CC.");
    }
    if (val < valField.min() || val > valField.max ())
    {
        throw std::runtime_error ("Invalid MIDI CC value.");
    }

    bc.setValue(ccField.bitOffset(index-1), ccField.bitLength(), cc);
    bc.setValue(valField.bitOffset(index-1), valField.bitLength(), val);
}

void Patch::print()
{
    auto dataCopy = data();
    BitDecoder bc (dataCopy);

    std::cout << "Name: ";
    Field curField = g_fields.at("ID_PATCH_NAME_");
    for (auto i = 0; i < curField.numFields(); ++i)
    {
        std::cout << char(bc.getValue(curField.bitOffset(i), curField.bitLength()));
    }
    std::cout << std::endl;

    const char *loopStates[] = { "-", "1", "2", "3", "4", "5", "6", "7", "8", "V" };

    std::cout << "Loops: ";
    curField = g_fields.at("ID_PATCH_LOOP_SW_LOOP_");
    for (auto i = 0; i < curField.numFields(); ++i)
    {
        std::cout << loopStates[bc.getValue(curField.bitOffset(i), curField.bitLength()) * (i + 1)];
    }
    curField = g_fields.at ("ID_PATCH_LOOP_SW_LOOP_V");
    std::cout << (bc.getValue(curField.bitOffset(), curField.bitLength()) ? "V" : "-");
    std::cout << std::endl;

    std::cout << "Input: ";
    curField = g_fields.at ("ID_PATCH_INPUT_SELECT");
    std::cout << curField.value(bc.getValue(curField.bitOffset(), curField.bitLength())); 
    std::cout << std::endl;

    std::cout << "Output: ";
    curField = g_fields.at ("ID_PATCH_OUTPUT_SELECT");
    std::cout << curField.value(bc.getValue(curField.bitOffset(), curField.bitLength())); 
    std::cout << std::endl;

    std::cout << "MIDI: ";
    curField = g_fields.at ("ID_PATCH_MIDI_TX_CH_");
    for (auto i = 0; i < curField.numFields(); ++i)
    {
        auto ch = bc.getValue(curField.bitOffset(i), curField.bitLength());
        if (ch > 1)
        {
            auto pcField = g_fields.at ("ID_PATCH_MIDI_PC_");
            auto pc = bc.getValue(pcField.bitOffset(i), pcField.bitLength());
            auto cc1Field = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_");
            auto cc1 = bc.getValue(cc1Field.bitOffset(i), cc1Field.bitLength());
            auto cc1ValField = g_fields.at ("ID_PATCH_MIDI_CTL1_CC_VAL_");
            auto cc1Val = bc.getValue(cc1ValField.bitOffset(i), cc1ValField.bitLength());
            auto cc2Field = g_fields.at ("ID_PATCH_MIDI_CTL2_CC_");
            auto cc2 = bc.getValue(cc2Field.bitOffset(i), cc2Field.bitLength());
            auto cc2ValField = g_fields.at ("ID_PATCH_MIDI_CTL2_CC_VAL_");
            auto cc2Val = bc.getValue(cc2ValField.bitOffset(i), cc2ValField.bitLength());
            std::cout << i+1 << ":(CH: " << ch;
            if (pc != 0) std::cout << " PC: " << pc;
            if (cc1 != 0) std::cout << " CC1: " << cc1-1 << " " << cc1Val;
            if (cc2 != 0) std::cout << " CC2: " << cc2-1 << " " << cc2Val;
            std::cout << ") ";
        }
    }
    std::cout << std::endl;
}

void Patch::writeFileData(std::ostream & s)
{
    auto dataCopy = data();
    BitDecoder bc (dataCopy);

    for (auto it = g_fields.begin(); it != g_fields.end(); ++it)
    {
        if (it->second.type() == Field::Patch)
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

void Patch::load(std::string filename)
{
    std::ifstream infile (filename);
    
    std::string line;
    std::getline(infile, line);
    if (line != "ES8cli patch file format 1")
    {
        std::cerr << "wrong file format <" << line << ">" << std::endl;
        throw std::runtime_error ("Wrong patch file format");
    }

    loadValues (infile);
}

void Patch::save(std::string filename)
{
    std::ofstream backupFile;
    backupFile.open (filename);
    backupFile << "ES8cli patch file format 1" << std::endl;
    writeFileData(backupFile);
    backupFile.close();
}

bool Patch::dataValid(std::vector<uint8_t> & data)
{
    return (data.size () == 250);
}

void Patch::setLoop(size_t i, bool state)
{
    BitCodec bc (writeableData());

    Field curField = g_fields.at ("ID_PATCH_LOOP_SW_LOOP_");

    if (i == 8)
    {
        curField = g_fields.at ("ID_PATCH_LOOP_SW_LOOP_V");
    }

    i = i % 8;

    bc.setValue(curField.bitOffset(i), curField.bitLength(), state ? 1 : 0);
}


void Patch::setInput(Command::Parameter iParam)
{
    BitCodec bc (writeableData());

    Field curField = g_fields.at ("ID_PATCH_INPUT_SELECT");

    unsigned i = 0;
    if (iParam.isNumber())
    {
        i = iParam.num()-1;
    }
    else
    {
        i = curField.value(iParam.str());
    }

    if (i < curField.min() || i > curField.max ())
    {
        throw std::runtime_error ("Invalid input select.");
    }
    
    bc.setValue(curField.bitOffset(), curField.bitLength(), i);
}

void Patch::setOutput(Command::Parameter oParam)
{
    BitCodec bc (writeableData());

    Field curField = g_fields.at ("ID_PATCH_OUTPUT_SELECT");

    unsigned o = 0;
    if (oParam.isNumber())
    {
        o = oParam.num()-1;
    }
    else
    {
        o = curField.value(oParam.str());
    }

    if (o < curField.min() || o > curField.max ())
    {
        throw std::runtime_error ("Invalid input select.");
    }
    
    bc.setValue(curField.bitOffset(), curField.bitLength(), o);
}

