/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "rtmidi-4.0.0/RtMidi.h"
#include "midi.hpp"

#include "helpers.h"

///@todo use unique_ptr<>

#include "midimessages.hpp"

std::vector<uint8_t> waitForMessage(RtMidiIn * midiin, const std::vector<uint8_t> & expect, const std::vector<uint8_t> & expectMask);

MIDI::MIDI (unsigned devin, unsigned devout) : m_devIn (devin), m_devOut (devout)
{
}


std::string MIDI::midiInName(unsigned i)
{
    RtMidiIn  *midiin = 0;
    try 
    {
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "ES8cli", 2000);
        return midiin->getPortName(i-1);
    }
    catch (const RtMidiError &error)
    {
        error.printMessage();
        throw std::runtime_error ("MIDI error");
    }
}

std::string MIDI::midiOutName(unsigned i)
{
    RtMidiOut  *midiOut = 0;
    try 
    {
        midiOut = new RtMidiOut();
        return midiOut->getPortName(i-1);
    }
    catch (const RtMidiError &error)
    {
        error.printMessage();
        throw std::runtime_error ("MIDI error");
    }
}

int MIDI::listMidiDevices()
{
    int result = 0;
    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    try 
    {
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "ES8cli", 2000);
        unsigned int nPorts = midiin->getPortCount();
        if (nPorts == 1)
        {
            std::cout << nPorts << " MIDI input port:" << std::endl;
        }
        else
        {
            std::cout << nPorts << " MIDI input ports:" << std::endl;
        }
        std::string portName;
        for (unsigned int i=0; i<nPorts; i++) 
        {
            portName = midiin->getPortName(i);
            std::cout << "  " << i+1 << ": " << portName << std::endl;
        }
        std::cout << std::endl;
        midiout = new RtMidiOut();
        nPorts = midiout->getPortCount();
        if (nPorts == 1)
        {
            std::cout << nPorts << " MIDI output port:" << std::endl;
        }
        else
        {
            std::cout << nPorts << " MIDI output ports:" << std::endl;
        }
        for (unsigned int i=0; i<nPorts; i++) 
        {
            portName = midiout->getPortName(i);
            std::cout << "  " << i+1 << ": " << portName << std::endl;
        }
        std::cout << std::endl;
    }
    catch (const RtMidiError &error)
    {
        error.printMessage();
        result = EXIT_FAILURE;
    }

    delete midiin;
    delete midiout;
    return result;
}

std::vector<uint8_t> waitForMessage(RtMidiIn * midiin, const std::vector<uint8_t> & expect, const std::vector<uint8_t> & expectMask)
{
    using namespace std::chrono_literals;
    std::vector<uint8_t> result;
    bool found = false;
    unsigned timeout = 1000;

    while(!found && timeout > 0)
    {
        std::vector<uint8_t> message;
        double stamp = midiin->getMessage(&message);
        int nBytes = message.size();
	if (nBytes >= expectMask.size())
        {   
            found = true;
            for (auto i = 0; i < expectMask.size(); ++i)
            {
                found &= (message[i] & expectMask[i]) == (expect[i] & expectMask[i]);
            }
            if (found)
            {
                result = message;
            }
        }
        else
        {
            std::this_thread::sleep_for(1ms);
            timeout--;
        }
    }

    return result;
}

void MIDI::sendPatch (unsigned patch, std::vector<uint8_t> data)
{
    using namespace std::chrono_literals;

    data = scrambleData(data);

    WritePageMessage wpm1 (14 + 2*patch, std::vector<uint8_t> (&data[0], &data[155]));
    WritePageMessage wpm2 (14 + 2*patch + 1, std::vector<uint8_t> (&data[155], &data[2*155]));

    RtMidiOut *midiout = 0;
    RtMidiIn *midiin = 0;

    try 
    {
        midiout = new RtMidiOut();
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "ES8cli", 2000);
    }
    catch (const RtMidiError &error) 
    {
        error.printMessage();
        throw std::runtime_error ("Could not open MIDI ports");
    }

    midiout->openPort(m_devOut-1);
    midiin->openPort(m_devIn-1);

    midiin->ignoreTypes(false, true, true);

    //std::cout << "Request device ID" << std::endl;
    RequestIdMessage reqID;
    auto message = reqID.getMessageData();
    midiout->sendMessage(&message);

    //std::cout << "Wait for device ID" << std::endl;
    std::vector<uint8_t> expect;
    std::vector<uint8_t> expectMask;
    expectMask.clear();
    expect.push_back (0xf0); expectMask.push_back (0xff);
    expect.push_back (0x7e); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0x00);
    expect.push_back (0x06); expectMask.push_back (0xff);
    expect.push_back (0x02); expectMask.push_back (0xff);
    expect.push_back (0x41); expectMask.push_back (0xff);
    expect.push_back (0x14); expectMask.push_back (0xff);
    expect.push_back (0x03); expectMask.push_back (0xff);

    auto result = waitForMessage (midiin, expect, expectMask);

    if (result.size() == 0)
    {
        throw std::runtime_error ("Could not communicate with ES-8");
    }
    std::this_thread::sleep_for(2ms);

    auto d1 = wpm1.getMessageData();
    writeFile ("debug_midimessage_1.bin", d1);
    midiout->sendMessage(&d1);
    std::this_thread::sleep_for(2ms);

    auto d2 = wpm2.getMessageData();
    writeFile ("debug_midimessage_1.bin", d2);
    midiout->sendMessage(&d2);
}

std::vector<uint8_t> MIDI::retrievePatch (unsigned patch, unsigned count)
{
    //std::cout << std::endl << "Retrieve patch " << patch << std::endl;
    using namespace std::chrono_literals;
    
    RtMidiOut *midiout = 0;
    RtMidiIn *midiin = 0;
    std::vector<uint8_t> expect;
    std::vector<uint8_t> expectMask;
    std::vector<uint8_t> result;

    try 
    {
        midiout = new RtMidiOut();
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "ES8cli", 2000);
    }
    catch (const RtMidiError &error) 
    {
        error.printMessage();
        throw std::runtime_error ("Could not open MIDI ports");
    }

    midiout->openPort(m_devOut-1);
    midiin->openPort(m_devIn-1);

    midiin->ignoreTypes(false, true, true);

    //std::cout << "Request device ID" << std::endl;
    RequestIdMessage reqID;
    auto message = reqID.getMessageData();
    midiout->sendMessage(&message);

    //std::cout << "Wait for device ID" << std::endl;
    expect.clear();
    expectMask.clear();
    expect.push_back (0xf0); expectMask.push_back (0xff);
    expect.push_back (0x7e); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0x00);
    expect.push_back (0x06); expectMask.push_back (0xff);
    expect.push_back (0x02); expectMask.push_back (0xff);
    expect.push_back (0x41); expectMask.push_back (0xff);
    expect.push_back (0x14); expectMask.push_back (0xff);
    expect.push_back (0x03); expectMask.push_back (0xff);
    result = waitForMessage (midiin, expect, expectMask);

    if (result.size() == 0)
    {
        throw std::runtime_error ("Could not communicate with ES-8");
    }

    std::this_thread::sleep_for(2ms);

    //std::cout << "Request patch" << std::endl;
    RequestDataMessage reqDat(14 + 2 * patch, 2*count);
    message = reqDat.getMessageData();
    midiout->sendMessage(&message);

    //std::cout << "Wait for patch" << std::endl;
    expect.clear();
    expectMask.clear();
    expect.push_back (0xf0); expectMask.push_back (0xff);
    expect.push_back (0x41); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0x00);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x14); expectMask.push_back (0xff);
    expect.push_back (0x12); expectMask.push_back (0xff);

    result = waitForMessage (midiin, expect, expectMask);

    if (result.size() == 0)
    {
        throw std::runtime_error ("Could not communicate with ES-8");
    }

    for (auto i = 0; i < 2 * (count) - 1; ++i) 
    {
        auto result2 = waitForMessage (midiin, expect, expectMask);
        if (result2.size() == 0)
        {
            throw std::runtime_error ("Could not communicate with ES-8");
        }


        result.insert(result.end(), result2.begin(), result2.end());
    }
    writeFile ("debug_incoming.bin", result);
    result = unscrambleData (result);
    return result;
}

std::vector<uint8_t> MIDI::retrieveSystem ()
{
    //std::cout << std::endl << "Retrieve system pages" << std::endl;
    using namespace std::chrono_literals;
    
    RtMidiOut *midiout = 0;
    RtMidiIn *midiin = 0;
    std::vector<uint8_t> expect;
    std::vector<uint8_t> expectMask;
    std::vector<uint8_t> result;

    try 
    {
        midiout = new RtMidiOut();
        midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "ES8cli", 2000);
    }
    catch (const RtMidiError &error) 
    {
        error.printMessage();
        throw std::runtime_error ("Could not open MIDI ports");
    }

    midiout->openPort(m_devOut-1);
    midiin->openPort(m_devIn-1);

    midiin->ignoreTypes(false, true, true);

    //std::cout << "Request device ID" << std::endl;
    RequestIdMessage reqID;
    auto message = reqID.getMessageData();
    midiout->sendMessage(&message);

    //std::cout << "Wait for device ID" << std::endl;
    expect.clear();
    expectMask.clear();
    expect.push_back (0xf0); expectMask.push_back (0xff);
    expect.push_back (0x7e); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0x00);
    expect.push_back (0x06); expectMask.push_back (0xff);
    expect.push_back (0x02); expectMask.push_back (0xff);
    expect.push_back (0x41); expectMask.push_back (0xff);
    expect.push_back (0x14); expectMask.push_back (0xff);
    expect.push_back (0x03); expectMask.push_back (0xff);
    result = waitForMessage (midiin, expect, expectMask);

    if (result.size() == 0)
    {
        throw std::runtime_error ("Could not communicate with ES-8");
    }

    std::this_thread::sleep_for(2ms);

    //std::cout << "Request system pages" << std::endl;
    RequestDataMessage reqDat(0);
    message = reqDat.getMessageData();
    midiout->sendMessage(&message);

    expect.clear();
    expectMask.clear();
    expect.push_back (0xf0); expectMask.push_back (0xff);
    expect.push_back (0x41); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0x00);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x00); expectMask.push_back (0xff);
    expect.push_back (0x14); expectMask.push_back (0xff);
    expect.push_back (0x12); expectMask.push_back (0xff);

    result = waitForMessage (midiin, expect, expectMask);

    if (result.size() == 0)
    {
        throw std::runtime_error ("Could not communicate with ES-8");
    }

    for (auto i = 0; i < 15; ++i) 
    {
        //std::cout << "Wait for patch (part " << (i+2) << ")" << std::endl;
        auto result2 = waitForMessage (midiin, expect, expectMask);
        if (result2.size() == 0)
        {
            throw std::runtime_error ("Could not communicate with ES-8");
        }

        result.insert(result.end(), result2.begin(), result2.end());
    }
    return result;
}

std::vector<uint8_t> MIDI::scrambleData(const std::vector<uint8_t> & dataIn)
{
    std::vector<uint8_t> result;
    size_t i = 0;
    for (auto block = 0; block < dataIn.size()/(125); ++block)
    {
        // reserve space for SysEx protocol
        for (size_t j = 0; j < 10; ++j)
        {
            result.push_back(0);
        }
        std::vector<uint8_t> temp;
    
        uint8_t msb = 0;
        for (size_t j = 0; j < 125; ++j, ++i)
        {
            temp.push_back (dataIn[i] & 0x7f);
            msb |= (dataIn[i] & 0x80) >> temp.size();
            if (temp.size() == 7 || j == 124)
            {
                result.push_back (msb);
                result.insert (result.end(), temp.begin(), temp.end());
                temp.clear();
                msb = 0;
            }
        }
        // reserve space for SysEx protocol
        for (size_t j = 0; j < 2; ++j)
        {
            result.push_back(0);
        }
    }

    return result;
}

std::vector<uint8_t> MIDI::unscrambleData(const std::vector<uint8_t> & dataIn)
{
    std::vector<uint8_t> result;

    for (auto block = 0; block < dataIn.size()/155; ++block)
    {
        std::vector<uint8_t> data (&dataIn[block * 155], &dataIn[block * 155] + 155);
        
        //std::cout << "Input data: " << data.size() << std::endl;
        //std::cout << std::hex;
        //std::cout << "Exclusive status: " << unsigned (data[0]) << std::endl;
        //std::cout << "Manufacturer ID: " << unsigned (data[1]) << std::endl;
        //std::cout << "Device ID: " << unsigned (data[2]) << std::endl;
        //std::cout << "Model ID: " << unsigned (data[3]) << " " << unsigned (data[4]) << " " << unsigned (data[5]) << " " << unsigned (data[6]) << " " << std::endl;
        //std::cout << "Command: " << unsigned (data[7]) << std::endl;
        //std::cout << "Message ID: " << unsigned (data[8]) << " " << unsigned(data[9]) << std::endl;

        unsigned checksum = 0;
        for (size_t i = 8; i < data.size()-2; ++i)
        {
            checksum += data[i];
        }
    
        //std::cout << "Checksum: " << unsigned (data[data.size()-2]) << std::endl;
        bool chkOk = (((unsigned (data[data.size()-2]) + checksum) & 0x7f) == 0);
        std::cout << "Checksum test: " << (chkOk ? "OK" : "FAIL") << std::endl;
        //std::cout << "End marker: " << unsigned (data[data.size()-1]) << std::endl;
        //std::cout << std::dec;
        if (!chkOk)
        {
            throw std::runtime_error ("MIDI data checksum failed");
        }
    
        std::vector<uint8_t> temp;
        for (size_t i = 10; i < data.size()-2; ++i)
        {
            if (temp.size() < 7)
            {
                temp.push_back(data[i]);
            }
            else
            {
                temp.push_back(data[i]);
                for (size_t j = 0; j < 7; ++j)
                {
                    result.push_back(temp[j+1] + ((temp[0] << (j+1)) & 0x80));
                }
                temp.clear();
            }
        }
    
        for (size_t j = 0; j < temp.size()-1; ++j)
        {
    	    result.push_back(temp[j+1] + ((temp[0] << (j+1)) & 0x80));
        }
    }
    return result;
}

