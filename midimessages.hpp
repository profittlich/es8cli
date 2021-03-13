/* Copyright (c) 2021 Martin Profittlich. All rights reserved. */
/* The file LICENSE contains more information about licensing. */

#pragma once

#include <vector>

/**
 * Message to be sent to ES-8 via MIDI.
 */
class MidiMessage
{
    public:
        /** Get the bytes to be sent via MIDI. */
        const std::vector<uint8_t> & getMessageData ();

    protected:
        /** Construct the MIDI message byte sequence. */
        virtual void constructMessage() = 0;

        /** Writable reference to the MIDI data byte buffer. */
        std::vector<uint8_t> & data();

    private:
        /** MIDI data byte buffer. */
        std::vector<uint8_t> m_message;
};

class WritePageMessage : public MidiMessage
{
    public:
        /**
         * Constructor.
         *
         * @param page First memory page to request.
         * @param data The page data to write.
         */
        WritePageMessage (unsigned startPage, const std::vector<uint8_t> & data);

    protected:
        void constructMessage () override;

    private:
        /** First memory page to request. */
        unsigned m_page;
        /** The data to write. */
        std::vector<uint8_t> m_pageData;
};

/**
 * Request a data dump from ES-8.
 */
class RequestDataMessage : public MidiMessage
{
    public:
        /**
         * Constructor.
         *
         * @param page First memory page to request.
         * @param len Number of pages to request.
         */
        RequestDataMessage (unsigned page, unsigned len=1);

    protected:
        void constructMessage () override;

    private:
        /** First memory page to request. */
        unsigned m_page;
        /** Number of pages to request. */
        unsigned m_length;
};

/**
 * Request the MIDI ID of the ES-8.
 */
class RequestIdMessage : public MidiMessage
{
    protected:
        void constructMessage () override;
};
