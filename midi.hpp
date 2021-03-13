/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include "midimessages.hpp"

/**
 * Handle MIDI communication.
 * 
 * Uses RtMidi internally.
 */
class MIDI
{
    public:
        /**
         * Constructor
         *
         * @param devin Index of the MIDI in device to use to communicate with the ES-8
         * @param devout Index of the MIDI in device to use
         */
        MIDI (unsigned devin, unsigned devout);

        /**
         * Retrieve one or more patches from the ES-8
         *
         * @param patch Patch number to start with
         * @param count Number of patches to retrieve
         */
        std::vector<uint8_t> retrievePatch (unsigned patch, unsigned count = 1);

        /**
         * Send one patch to the ES-8
         *
         * @param patch Patch number to start with
         * @param data The patch data
         */
        void sendPatch (unsigned patch, std::vector<uint8_t> data);

        /**
         * Retrieve global parameters from the ES-8
         */
        std::vector<uint8_t> retrieveSystem ();

        /** Display available MIDI devices and their indexes. */
        static int listMidiDevices();

        /** Get the name of a MIDI input port.
         *
         * @param i Index of the port
         */
        static std::string midiInName(unsigned i);

        /** Get the name of a MIDI output port.
         *
         * @param i Index of the port
         */
        static std::string midiOutName(unsigned i);

        /** Unscramble 7-bit encoded data to 8-bit. */
        static std::vector<uint8_t> unscrambleData(const std::vector<uint8_t> & dataIn);

        /** Scramble 8-bit data to 7-bit. */
        static std::vector<uint8_t> scrambleData(const std::vector<uint8_t> & dataIn);

    private:
        /** Index of the MIDI in device to use to communicate with the ES-8. */
        unsigned m_devIn;
        /** Index of the MIDI out device to use to communicate with the ES-8. */
        unsigned m_devOut;
};

