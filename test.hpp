/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>
#include <map>
#include <iostream>

void testSystemStruct()
{
    std::vector<unsigned> testMap (8 * 125);
    std::map<Field::Type, unsigned> numBits;
    numBits[Field::Globals] = 0;
    numBits[Field::Patch] = 0;

    for (auto it = g_fields.begin(); it != g_fields.end(); ++it)
    {
       for (auto i = 0; i < it->second.numFields(); ++i)
       {
           numBits[it->second.type()] += it->second.bitLength();
           if (it->second.type() == 0)
           {
               for (auto j = 0; j < it->second.bitLength(); ++j)
               {
                   testMap[it->second.bitOffset(i) + j]++;
               }
           }
       }
    }

    for (auto i = 0; i < 125; ++i)
    {
        std::cout << i << ": ";
        for (auto j = 0; j < 8; ++j)
        {
            if (testMap[i*8+j] != 1)
            {
                std::cerr << " Corrupted value map" << std::endl;
            }
            std::cout <<  unsigned(testMap[i*8+j]);
        }
        std::cout << std::endl;
    }
}

void testStruct()
{
    std::vector<unsigned> testMap (8 * 250);
    std::map<Field::Type, unsigned> numBits;
    numBits[Field::Globals] = 0;
    numBits[Field::Patch] = 0;
    for (auto it = g_fields.begin(); it != g_fields.end(); ++it)
    {
       for (auto i = 0; i < it->second.numFields(); ++i)
       {
           //std::cout << it->id() << " " << i << std::endl;
           numBits[it->second.type()] += it->second.bitLength();
           if (it->second.type() == Field::Patch)
           {
               for (auto j = 0; j < it->second.bitLength(); ++j)
               {
                   testMap[it->second.bitOffset(i) + j]++;
               }
           }
       }
    }

    for (auto i = 0; i < 250; ++i)
    {
        std::cout << i << ": ";
        for (auto j = 0; j < 8; ++j)
        {
            if (testMap[i*8+j] != 1)
            {
                std::cerr << " Corrupted value map" << std::endl;
            }
            std::cout <<  unsigned(testMap[i*8+j]);
        }
        std::cout << std::endl;
    }
}

void testScramble(std::vector<uint8_t> d)
{
    auto u = MIDI::unscrambleData(d);
    auto re = MIDI::scrambleData(u);
    for (auto block = 0; block < d.size()/155; ++block)
    {
        for (size_t i = 10; i < 155-2; ++i)
        {
            if (d[block*155+i] != re[block*155+i]) std::cout << "(" << i << ": " << std::hex << unsigned (d[block*155+i]) << " vs. " << unsigned (re[block*155+i]) << ") " << std::dec;
            else std::cout << ".";
        }
    }
    std::cout << std::endl;
    auto reu = MIDI::unscrambleData(re);
    for (auto block = 0; block < u.size()/125; ++block)
    {
        for (size_t i = 0; i < 125; ++i)
        {
            if (u[block*125+i] != reu[block*125+i]) std::cout << "(" << i << ": " << std::hex << unsigned (u[block*125+i]) << " vs. " << unsigned (reu[block*125+i]) << ") " << std::dec;
            else std::cout << ".";
        }
    }
    std::cout << std::endl;
    std::cout << "Scramble test: " << d.size () << " " << u.size() << " " << re.size () << " " << reu.size () << std::endl;
}

void testScrambleRev(std::vector<uint8_t> d)
{
    auto u = MIDI::scrambleData(d);

    WritePageMessage wpm1 (0x66, std::vector<uint8_t> (&u[0], &u[155]));
    auto m1 = wpm1.getMessageData();

    WritePageMessage wpm2 (0x66, std::vector<uint8_t> (&u[155], &u[2*155]));
    auto m2 = wpm2.getMessageData();

    m1.insert (m1.end(), m2.begin(), m2.end());

    auto re = MIDI::unscrambleData(m1);
    for (auto block = 0; block < d.size()/125; ++block)
    {
        std::cout <<std::endl << "block " << block << std::endl;
        for (size_t i = 0; i < 125; ++i)
        {
            if (d[block*125+i] != re[block*125+i]) std::cout << "(" << i << ": " << std::hex << unsigned (d[block*125+i]) << " vs. " << unsigned (re[block*125+i]) << ") " << std::dec;
            else std::cout << ".";
        }
    }
    std::cout << std::endl;
    auto reu = MIDI::scrambleData(re);

    WritePageMessage wpm3 (0x66, std::vector<uint8_t> (&reu[0], &reu[155]));
    auto m3 = wpm3.getMessageData();

    WritePageMessage wpm4 (0x66, std::vector<uint8_t> (&reu[155], &reu[2*155]));
    auto m4 = wpm4.getMessageData();

    m3.insert (m3.end(), m4.begin(), m4.end());

    for (auto block = 0; block < u.size()/155; ++block)
    {
        std::cout <<std::endl << "block " << block << std::endl;
        for (size_t i = 0; i < 155; ++i)
        {
            if (m1[block*155+i] != m3[block*155+i]) std::cout << "(" << i << ": " << std::hex << unsigned (m1[block*155+i]) << " vs. " << unsigned (m3[block*155+i]) << ") " << std::dec;
            else std::cout << ".";
        }
    }
    std::cout << std::endl;
    std::cout << "Reverse scramble test: " << d.size () << " " << u.size() << " " << re.size () << " " << m3.size () << std::endl;
    writeFile ("scram.bin", m3);
}

