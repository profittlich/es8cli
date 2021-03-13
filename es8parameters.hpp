/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

///@todo document

typedef std::map<unsigned, std::string> AliasMap;

/**
 * Describe a field in the ES-8 binary configuration.
 */
class Field
{
    public:
        typedef enum { Globals, Patch } Type;
        Field (Type type, std::string id, size_t bitOffset, size_t bitLength, unsigned min, unsigned max, unsigned numFields=1, const AliasMap & alias = AliasMap());

        Type type();
        std::string id();
        
        size_t bitOffset(size_t index = 0);

        size_t bitLength();
        unsigned min();
        unsigned max();
        size_t numFields();
        std::string value(unsigned value);
        unsigned value(std::string value);

    private:
        Type m_type;
        std::string m_id;
        size_t m_bitOffset;
        size_t m_bitLength;
        unsigned m_min;
        unsigned m_max;
        size_t m_numFields;
        AliasMap m_alias;
};

extern std::map<std::string, Field> g_fields;

