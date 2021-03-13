/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <vector>

#include "bitcoder.hpp"

#include <iostream>


BitDecoder::BitDecoder (const std::vector<uint8_t> & data) : m_data(data)
{
}

unsigned BitDecoder::getValue(size_t off, size_t len)
{
    auto bitData = bitifyData (m_data);
    unsigned result = 0;
    for (size_t i = 0; i < len; ++i)
    {
	result += bitData[off+i] << (len-1-i);
    }
    return result;
}

BitCodec::BitCodec (std::vector<uint8_t> & data) : BitDecoder(data), m_encData(data)
{
}

void BitCodec::setValue(size_t off, size_t len, unsigned bits)
{
    auto bitData = bitifyData (m_encData);
    for (size_t i = 0; i < len; ++i)
    {
	bitData[off+i] = (bits >> (len-1-i)) & 1;
    }
    m_encData = unBitifyData(bitData);
}

std::vector<uint8_t> BitDecoder::bitifyData(const std::vector<uint8_t> & data)
{
    std::vector<uint8_t> result;

    for (size_t i = 0; i < data.size(); ++i)
    {
        for (size_t j = 0; j < 8; ++j)
        {
            result.push_back ((data[i] >> (7-j)) & 0x01);
        }
    }

    return result;
}

std::vector<uint8_t> BitDecoder::unBitifyData(const std::vector<uint8_t> & data)
{
    std::vector<uint8_t> result;

    for (size_t i = 0; i < data.size() / 8; ++i)
    {
        uint8_t temp = 0;
        for (size_t j = 0; j < 8; ++j)
        {
            temp |= data[i * 8 + j] << (7-j);
        }
        result.push_back(temp);
    }

    return result;
}

