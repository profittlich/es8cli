/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>

/**
 * Handle values in a non-uniform bitstream.
 */
class BitDecoder
{
    public:

        /**
         * Constructor
         *
         * @param data Binary data blob.
         */
        BitDecoder (const std::vector<uint8_t> & data);

        /**
         * Get a value from a specific position, with a specific bit depth.
         *
         * @param off Position of the value in bits.
         * @param len Bit depth of the value.
         * @return The value stored at the specified position.
         */
        virtual unsigned getValue(size_t off, size_t len);

    protected:
        /**
         * Convert a byte array to a bit array.
         *
         * @param data Binary data blob.
         * @return Converted binary blob (8x the size, one bit per index).
         */
        std::vector<uint8_t> bitifyData(const std::vector<uint8_t> & data);

        /**
         * Convert a bit array to a byte array.
         *
         * @param data Converted binary blob (8x the size, one bit per index).
         * @param data Binary data blob.
         */
        std::vector<uint8_t> unBitifyData(const std::vector<uint8_t> & data);

    private:
        const std::vector<uint8_t> & m_data;
};

class BitCodec : public BitDecoder
{
    public:
        /**
         * Constructor
         *
         * @param data Binary data blob.
         */
        BitCodec (std::vector<uint8_t> & data);

        /**
         * Set a value at a specific position, with a specific bit depth.
         *
         * @param data Bit array, one bit per index.

         * @param off Position of the value in bits.
         * @param len Bit depth of the value.
         * @param bits The value to be stored at the specified position.
         */
        virtual void setValue(size_t off, size_t len, unsigned bits);
    private:
        std::vector<uint8_t> & m_encData;
};
