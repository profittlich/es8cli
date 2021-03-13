/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include "execute.hpp"
#include <iostream>
#include "midi.hpp"
#include "patch.hpp"

bool validateProgram(CmdLineParameters config)
{
    std::cout << "Program validation not implemented." << std::endl;
///@todo Implement program validation.
// check for MIDI
// check for data.size() for Store etc.
    return true;
}

void runProgram(CmdLineParameters config)
{
    std::vector<uint8_t> data; 
    size_t count = 1;
    for (auto it : config.commands)
    {
        std::cout << count++ << ": ";
        executeCommand(it, data, config);
        std::cout << std::endl;
    }
}


void executeCommand (const Command & cmd, std::vector<uint8_t> & data, const CmdLineParameters & config)
{
    Patch ptch;
    switch (cmd.command ())
    {
        case CommandType::Select:
            std::cout << "=== Select " << cmd.parameter(0).str() << " ===" << std::endl;
            if(!cmd.parameter(0).isNumber())
            { 
                if (cmd.parameter(0).str() == "globals")
                {
                    std::cout << "Globals not implemented." << std::endl;
                }
                else
                {
                    ptch.load(cmd.parameter(0).str());
                    data = ptch.data();
                }
            }
            else
            {
                if (config.hasMidi)
                {
                    MIDI midi (config.midiin, config.midiout);
	            data = midi.retrievePatch (cmd.parameter(0).num());
                    ptch.setData(data);
                }
                else
                {
                    std::cout << "Error: No MIDI ports selected." << std::endl;
                }
            }
            break;
        case CommandType::Display:
            std::cout << "=== Display ===" << std::endl;
            ptch.setData(data);
            ptch.print();
            break;
        case CommandType::View:
            std::cout << "=== View " << cmd.parameter(0).str() << " ===" << std::endl;
            executeCommand (Command(CommandType::Select, cmd.parameter(0)), data, config);
            executeCommand (Command(CommandType::Display), data, config);
            break;
        case CommandType::Copy:
            std::cout << "=== Copy " << cmd.parameter(0).str() << " to " << cmd.parameter(1).str() << " ===" << std::endl;
            executeCommand (Command(CommandType::Select, cmd.parameter(0)), data, config);
            executeCommand (Command(CommandType::Store, cmd.parameter(1)), data, config);
            break;
        case CommandType::Store:
            std::cout << "=== Store " << cmd.parameter(0).str() << " ===" << std::endl;
            if(!cmd.parameter(0).isNumber())
            { 
                if (cmd.parameter(0).str() == "globals")
                {
                    std::cout << "Globals not implemented." << std::endl;
                }
                else
                {
    	            ptch.setData(data);
                    ptch.save(cmd.parameter(0).str());
                }
            }
            else
            {
                if (config.hasMidi)
                {
                    MIDI midi (config.midiin, config.midiout);
                    midi.sendPatch (cmd.parameter(0).num(), data);
                }
                else
                {
                    std::cout << "Error: No MIDI ports selected." << std::endl;
                }
            }
            break;

        case CommandType::Name:
            std::cout << "=== Name " << cmd.parameter(0).str() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setName(cmd.parameter(0).str());
            data = ptch.data();
            break;

        case CommandType::PatchMidiChannel:
            std::cout << "=== PatchMidiChannel " << cmd.parameter(0).str() << " " << cmd.parameter(1).str() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setPatchMidiChannel(cmd.parameter(0).num(), cmd.parameter(1));
            data = ptch.data();
            break;

        case CommandType::PatchMidiPC:
            std::cout << "=== PatchMidiPC " << cmd.parameter(0).str() << " " << cmd.parameter(1).str() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setPatchMidiPC(cmd.parameter(0).num(), cmd.parameter(1));
            data = ptch.data();
            break;

        case CommandType::PatchMidiCC:
            std::cout << "=== PatchMidiCC " << cmd.parameter(0).str() << " " << cmd.parameter(1).str() << " " << cmd.parameter(2).str() << " " << cmd.parameter(3).num() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setPatchMidiCC(cmd.parameter(0).num(), cmd.parameter(1).num(), cmd.parameter(2), cmd.parameter(3).num());
            data = ptch.data();
            break;

        case CommandType::Loops:
            {
                std::cout << "=== Loops " << cmd.parameter(0).str() << " ===" << std::endl;
                std::string loops = cmd.parameter(0).str();
    	    ptch.setData(data);
                for (size_t i = 0; i < 9; ++i)
                {
    		ptch.setLoop(i, false);
                }
                if (loops.size () > 0 && loops[0] != '-')
                {
                    for (size_t i = 0; i < loops.size(); ++i)
                    {
                        switch (loops[i])
                        {
                            case 'V':
                            case 'v':
                                ptch.setLoop(8, true);
                                break;
                            case '1':
                            case '2':
                            case '3':
                            case '4':
                            case '5':
                            case '6':
                            case '7':
                            case '8':
                                ptch.setLoop(loops[i] - '1', true);
                                break;
                            default:
                                throw std::runtime_error ("Invalid loop identifier");
                        }
                    }
                }
                data = ptch.data();
            }
            break;

        case CommandType::Input:
            std::cout << "=== Input " << cmd.parameter(0).str() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setInput(cmd.parameter(0));
            data = ptch.data();
            break;

        case CommandType::Output:
            std::cout << "=== Input " << cmd.parameter(0).str() << " ===" << std::endl;
	    ptch.setData(data);
            ptch.setOutput(cmd.parameter(0));
            data = ptch.data();
            break;

        case CommandType::None:
        default:
            throw std::logic_error ("Invalid command encountered. This is a bug.");
    }
}
