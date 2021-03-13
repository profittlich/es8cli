/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <list>

#include "patch.hpp"
#include "globals.hpp"
#include "bitcoder.hpp"
#include "es8parameters.hpp"

#include "midi.hpp"
#include "helpers.h"
#include "test.hpp"
#include "commands.hpp"
#include "commandline.hpp"
#include "execute.hpp"


void printLegal()
{
    std::cout << "ES8cli 0.1 - Copyright (c) 2020 Martin Profittlich" << std::endl;
    std::cout << "Licensed under a MIT-style license." << std::endl;
    std::cout << std::endl;
}

void printUsage()
{
    std::cout << "usage: es8cli [--help|--list-midi|<options> <commands>]" << std::endl;
}

void printHelp()
{
    std::cout << std::endl;
    std::cout << "  --help:         Show this help text" << std::endl;
    std::cout << "  --list-midi:    List MIDI input and output devices" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl << std::endl;
    std::cout << "  --verbose:      Output more information" << std::endl;
    std::cout << std::endl;
    std::cout << "  --midi-in <N>:  Index of the MIDI input port to use" << std::endl;
    std::cout << "  --midi-out <N>: Index of the MIDI output port to use" << std::endl;
    std::cout << std::endl;

    std::cout << "Commands:" << std::endl << std::endl;
    std::cout << "  select [patch|filename]:                          Select a patch from the ES-8 or a file" << std::endl;
    std::cout << "  copy [patch|filename] [patch|filename]:           Copy a patch from one location to another (the ES-8 or a file)" << std::endl;
    std::cout << "  store [patch|filename]:                           Store the currently used patch to the ES-8 or a file" << std::endl;
    std::cout << "  view [patch|filename]:                            View information about a patch from the ES-8 or a file" << std::endl;
    std::cout << "  display:                                          View information about the currently used patch" << std::endl;
    std::cout << "  name [patch name]:                                Set the name of the patch (32 characters max.)" << std::endl;
    std::cout << "  loops [-|12345678V]:                              Set loops on/off ('-' for no loops on, '34V' for loops 3, 4 and V on)" << std::endl;
    std::cout << "  input [1|2]:                                      Set input to use" << std::endl;
    std::cout << "  output [1|2|BOTH]:                                Set output(s) to use" << std::endl;
    std::cout << "  patchmidichannel [index] [channel|OFF]:           Set the MIDI channel for a patch MIDI setting" << std::endl;
    std::cout << "  patchmidipc [index] [PC|OFF]:                     Set the program change for a patch MIDI setting" << std::endl;
    std::cout << "  patchmidicc [index] [Ctl-index] [CC|OFF] [value]: Set a CC for a patch MIDI setting " << std::endl;
    std::cout << std::endl;

    std::cout << "Examples:" << std::endl << std::endl;

    std::cout << "  Show information about patch 44:" << std::endl;
    std::cout << "    view 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Change the name of patch 44 on the ES-8:" << std::endl;
    std::cout << "    select 44 name \"New name\" store 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Backup a patch:" << std::endl;
    std::cout << "    copy 44 mybackup.es8" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Restore a patch:" << std::endl;
    std::cout << "    copy mybackup.es8 44 " << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Duplicate a patch on the ES-8:" << std::endl;
    std::cout << "    copy 44 45" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Change the input of a patch:" << std::endl;
    std::cout << "    select 44 input 2 store 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Load a patch from a file and store it to two different patches with differnt loops activated:" << std::endl;
    std::cout << "    select mybackup.es8 loops 146 store 44 loops 148V store 45" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Send a program change and CC on MIDI channel 5 when activating a patch:" << std::endl;
    std::cout << "    select 44 patchmidichannel 1 5 patchmidipc 1 23 patchmidicc 1 1 85 127 store 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Also send a program change and CC on MIDI channel 7 when activating the patch:" << std::endl;
    std::cout << "    select 44 patchmidichannel 2 7 patchmidipc 2 3 patchmidicc 2 1 11 66 store 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Do not send a CC:" << std::endl;
    std::cout << "    select 44 patchmidicc 1 1 OFF 0 store 44" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "  Do not use patch MIDI block 1 at all:" << std::endl;
    std::cout << "    select 44 patchmidichannel 1 OFF store 44" << std::endl;
    std::cout << "" << std::endl;
}



int main(int argc, char *argv[])
{
    CmdLineParameters cmd;

    try
    {
        parseCommandLine (cmd, argc, argv);
        if (cmd.midiin > 0 && cmd.midiout > 0)
        {
            std::cout << "Using MIDI input port " << MIDI::midiInName(cmd.midiin) << std::endl;
            std::cout << "Using MIDI output port " << MIDI::midiOutName(cmd.midiout) << std::endl;
        }
        switch (cmd.mode)
        {
            case CmdLineParameters::Help:
                printLegal();
                printUsage();
                printHelp();
                return 0;
    
            case CmdLineParameters::ListMidi:
                MIDI::listMidiDevices();
                return 0;
    
            case CmdLineParameters::None:
            case CmdLineParameters::Usage:
                printUsage();
                return 0;
    
            case CmdLineParameters::SelfTest:
                testSystemStruct();
                testStruct();
                return 0;
    
            case CmdLineParameters::Run:
                if (validateProgram(cmd))
                {
                    runProgram(cmd);
                }
                else
                {
                    std::cout << "Invalid input: No commands executed." << std::endl;
                }
                break;
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << "Error: " << e.what () << std::endl;
    }
    return 0;
}


