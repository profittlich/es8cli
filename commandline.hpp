/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <string>
#include <list>
#include "helpers.h"
#include "commands.hpp"

struct CmdLineParameters
{
    enum { Usage, Help, ListMidi, SelfTest, Run, None } mode = None;
    unsigned midiin=0;
    unsigned midiout=0;
    bool unscramble = false;
    bool verbose = false;
    bool rawFile = false;
    bool hasMidi = false;
    std::list<Command> commands;
};

void parseCommandLine (CmdLineParameters & config, int argc, char *argv[]);
