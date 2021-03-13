/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include "es8data.hpp"
#include "commands.hpp"

///@todo document

class Patch : public ES8Data
{
    public:
        Patch ();

        void print () override;
        void load(std::string filename) override;
        void save(std::string filename) override;

        void setName(std::string name);
        void setPatchMidiChannel(size_t index, Command::Parameter channel);
        void setPatchMidiPC(size_t index, Command::Parameter pc);
        void setPatchMidiCC(size_t index, size_t ccindex, Command::Parameter cc, unsigned value);
        void setInput(Command::Parameter i);
        void setOutput(Command::Parameter o);
        void setLoop(size_t i, bool state);

    protected:
        void writeFileData (std::ostream & s) override;
        bool dataValid(std::vector<uint8_t> & data) override;
};

