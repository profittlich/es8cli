/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>
#include <ostream>

///@todo document

class ES8Data
{
    public:
        virtual void print () = 0;
        virtual void printVerbose ();
        virtual void load(std::string filename) = 0;
        virtual void save(std::string filename) = 0;
        void setData (std::vector<uint8_t> & data);
        const std::vector<uint8_t> data();

    protected:
        virtual bool dataValid(std::vector<uint8_t> & data) = 0;
        virtual void writeFileData (std::ostream & s) = 0;
        void loadValues(std::ifstream & infile);
        std::vector<uint8_t> & writeableData();

    private:
        std::vector<uint8_t> m_data;
};

