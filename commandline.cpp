/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include "commandline.hpp"

void parseCommandLine (CmdLineParameters & config, int argc, char *argv[])
{
    std::list <std::string> clparameters;
    auto pos = clparameters.end();

    for (auto p = argv + 1; p != argv+argc; ++p)
    {
        clparameters.push_back (*p);
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--midi-in"))) != clparameters.end())
    {
        auto prev = pos++;
        if (pos != clparameters.end())
        {
            std::stringstream conv(*pos);
            conv >> config.midiin;
            clparameters.erase(prev);
            clparameters.erase(pos);
        }
    }
    
    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--midi-out"))) != clparameters.end())
    {
        auto prev = pos++;
        if (pos != clparameters.end())
        {
            std::stringstream conv(*pos);
            conv >> config.midiout;
	    clparameters.erase(prev);
            clparameters.erase(pos);
        }
    }

    if (config.midiin && config.midiout)
    {
        config.hasMidi = true;
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--raw"))) != clparameters.end())
    {
        config.rawFile = true;
        clparameters.erase(pos);
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--verbose"))) != clparameters.end())
    {
        config.verbose = true;
        clparameters.erase(pos);
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--unscramble"))) != clparameters.end())
    {
        config.unscramble = true;
        clparameters.erase(pos);
    }

    // Modes

    config.mode = CmdLineParameters::Run;

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--list-midi"))) != clparameters.end())
    {
        config.mode = CmdLineParameters::ListMidi;
        clparameters.erase(pos);
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--self-test"))) != clparameters.end())
    {
        config.mode = CmdLineParameters::SelfTest;
        clparameters.erase(pos);
    }

    if ((pos = std::find (clparameters.begin(), clparameters.end(), std::string ("--help"))) != clparameters.end())
    {
        config.mode = CmdLineParameters::Help;
        clparameters.erase(pos);
    }

    if (clparameters.size () == 0 && config.mode == CmdLineParameters::Run)
    {
        config.mode = CmdLineParameters::Usage;
    }

    // Commands

    if (config.mode == CmdLineParameters::Run)
    {
        unsigned paramCount = 0;
        Command curCommand;
        for (auto c : clparameters)
        {
            if (paramCount == 0) // New command
            {
                if (c == "view") 
                {
                    curCommand = Command (CommandType::View);
                    paramCount = 1;
                }
                else if (c == "select") 
                {
                    curCommand = Command (CommandType::Select);
                    paramCount = 1;
                }
                else if (c == "display") 
                {
                    curCommand = Command (CommandType::Display);
                    paramCount = 0;
                }
                else if (c == "copy") 
                {
                    curCommand = Command (CommandType::Copy);
                    paramCount = 2;
                }
                else if (c == "store") 
                {
                    curCommand = Command (CommandType::Store);
                    paramCount = 1;
                }
                else if (c == "name") 
                {
                    curCommand = Command (CommandType::Name);
                    paramCount = 1;
                }
                else if (c == "loops") 
                {
                    curCommand = Command (CommandType::Loops);
                    paramCount = 1;
                }
                else if (c == "input") 
                {
                    curCommand = Command (CommandType::Input);
                    paramCount = 1;
                }
                else if (c == "output") 
                {
                    curCommand = Command (CommandType::Output);
                    paramCount = 1;
                }
                else if (c == "patchmidichannel") 
                {
                    curCommand = Command (CommandType::PatchMidiChannel);
                    paramCount = 2;
                }
                else if (c == "patchmidipc") 
                {
                    curCommand = Command (CommandType::PatchMidiPC);
                    paramCount = 2;
                }
                else if (c == "patchmidicc") 
                {
                    curCommand = Command (CommandType::PatchMidiCC);
                    paramCount = 4;
                }
                else 
                {
                    throw std::runtime_error (std::string ("Unknown command: ") + c);
                }
            }
            else // add parameter to command
            {
                Command::Parameter p;
                p.setStr(c);
                curCommand.addParameter(p);
                paramCount--;
            }

            if (paramCount == 0)
            {
                 config.commands.push_back(curCommand);
                 curCommand.clear();
            }
        }

        if (paramCount != 0)
        {
            throw std::runtime_error ("Parameter missing in last command");
        }
    }

}
