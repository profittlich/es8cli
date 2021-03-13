/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "helpers.h"

///@todo: Display vs. View -> better naming
typedef enum { None, Select, Display, View, Copy, Store, Name, PatchMidiChannel, PatchMidiPC, PatchMidiCC, Input, Output, Loops } CommandType;

class Command
{
    public:
        class Parameter 
        {
            public:
                void setStr(std::string s) 
                {
                    if (onlyDigits(s))
                    {
                        m_isNumber = true;
                        m_str = s;
                        std::stringstream conv(s);
                        conv >> m_num;
                    }
                    else
                    {
                        m_isNumber = false;
                        m_str = s;
                        m_num = UINT_MAX;
                    }
                }
    
                void setNum(unsigned n) 
                {
                    m_isNumber = true;
                    m_num = n;
                    std::stringstream conv;
                    conv << m_num;
                    m_str = conv.str();
                }
    
                std::string str() const
                {
                    return m_str;
                }
    
                unsigned num() const
                {
                    if (!isNumber()) throw std::logic_error ("Not a number parameter");
                    return m_num;
                }
    
                bool isNumber () const
                {
                    return m_isNumber;
                }
    
            private:
                bool m_isNumber;
                std::string m_str;
                unsigned m_num;
        };

        Command (CommandType t, Parameter p1, Parameter p2)
        {
            clear();
            m_command = t;
            addParameter (p1);
            addParameter (p2);
        }
    
        Command (CommandType t, Parameter p1)
        {
            clear();
            m_command = t;
            addParameter (p1);
        }
    
        Command (CommandType t)
        {
            clear();
            m_command = t;
        }
    
        Command ()
        {
            clear();
        }
    
        void clear()
        {
            m_command = None;
            m_params.clear();
        }

        CommandType command () const
        {
            return m_command;
        }

        void setCommand (CommandType c)
        {
            m_command = c;
        }

        Parameter parameter(size_t i) const
        {
            return m_params.at(i);
        }

        Parameter & parameter(size_t i)
        {
            return m_params.at(i);
        }

        void addParameter(Parameter p)
        {
            m_params.push_back (p);
        }

    private:
        CommandType m_command;
        std::vector<Parameter> m_params;
};
