/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include "midimessages.hpp"

#include <iostream>

const std::vector<uint8_t> & MidiMessage::getMessageData ()
{
    constructMessage ();
    return m_message;
}

std::vector<uint8_t> & MidiMessage::data()
{
    return m_message;
}

WritePageMessage::WritePageMessage (unsigned startPage, const std::vector<uint8_t> & data) : m_page (startPage), m_pageData (data) 
{
}

void WritePageMessage::constructMessage ()
{
    if (m_pageData.size() != 155)
    {
        throw std::logic_error ("Invalid page size for writing");
    }

    uint8_t pageHi = (m_page >> 7) & 0x7f;
    uint8_t pageLo = m_page & 0x7f;
    data().clear();
    data() = m_pageData;
    data()[0] = (0xf0); // SysEx
    data()[1] = (0x41); // Roland
    data()[2] =  (0x00); // Device ID
    data()[3] =  (0x00); // Model ID
    data()[4] =  (0x00); // Model ID
    data()[5] =  (0x00); // Model ID
    data()[6] =  (0x14); // Model ID
    data()[7] =  (0x12); // Transmit data/DT1
    data()[8] =  (pageHi); // Start page
    data()[9] =  (pageLo); // Start page
    unsigned checksum = 0;
    for (size_t i = 10; i < 153; ++i)
    {
        checksum += m_pageData[i];
        data()[i] = m_pageData[i];
    }
    data()[155-2] = (0x80 - ((checksum + pageHi + pageLo) & 0x7f)); // Checksum
    data()[155-1] = (0xf7); // End SysEx
}

RequestDataMessage::RequestDataMessage (unsigned page, unsigned len) : m_page (page), m_length (len) 
{
}

void RequestDataMessage::constructMessage ()
{
    uint8_t pageHi = (m_page >> 7) & 0x7f;
    uint8_t pageLo = m_page & 0x7f;
    uint8_t lengthHi = ((m_length-1) >> 7) & 0x7f;
    uint8_t lengthLo = (m_length-1) & 0x7f;
    data().clear();
    data().push_back (0xf0); // SysEx
    data().push_back (0x41); // Roland
    data().push_back (0x00); // Device ID
    data().push_back (0x00); // Model ID
    data().push_back (0x00); // Model ID
    data().push_back (0x00); // Model ID
    data().push_back (0x14); // Model ID
    data().push_back (0x11); // Request data/DT1
    data().push_back (pageHi); // Start page
    data().push_back (pageLo); // Start page
    data().push_back (lengthHi); // Additional pages
    data().push_back (lengthLo); // Additional pages
    data().push_back (0x80 - ((pageHi + pageLo + lengthHi + lengthLo) & 0x7f)); // Checksum
    data().push_back (0xf7); // End SysEx
}

void RequestIdMessage::constructMessage ()
{
    data().clear();
    data().push_back (0xf0); // SysEx
    data().push_back (0x7e); // Universal message
    data().push_back (0x7f); // Device ID
    data().push_back (0x06); // General Information
    data().push_back (0x01); // ID request
    data().push_back (0xf7); // End SysEx
}
