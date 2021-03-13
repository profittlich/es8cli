/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include "es8data.hpp"

///@todo document

class Globals : public ES8Data
{
    public:
        Globals ();
        void print () override;
        void load(std::string filename) override;
        void save(std::string filename) override;

    protected:
        void writeFileData (std::ostream & s) override;
        bool dataValid(std::vector<uint8_t> & data) override;
};

