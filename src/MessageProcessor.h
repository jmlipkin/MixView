#pragma once

#include "ChannelStrip.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

class MessageProcessor
{
private:
    InputChannelStrip *channels;
    InputChannelStrip *auxins;
    InputChannelStrip *buses;
    ChannelStrip *matrices;
    ChannelStrip *dcas;

    juce::Array<juce::OSCMessage> msg_buffer[2048];

public:
    MessageProcessor()
    {
        channels = new InputChannelStrip[32];
        auxins = new InputChannelStrip[8];
        buses = new InputChannelStrip[16];
        matrices = new ChannelStrip[6];
        dcas = new ChannelStrip[8];
    }
    ~MessageProcessor()
    {
        delete[] channels;
        delete[] auxins;
        delete[] buses;
        delete[] matrices;
        delete[] dcas;

        channels = nullptr;
        auxins = nullptr;
        buses = nullptr;
        matrices = nullptr;
        dcas = nullptr;
    }

    juce::String arg_to_str(const juce::OSCArgument &arg)
    {
        switch (arg.getType())
        {
        case 'f':
            return juce::String(arg.getFloat32());
        case 'i':
            return juce::String(arg.getInt32());
        case 's':
            return arg.getString();

        default:
            return juce::String("TYPE NOT HANDLED");
            break;
        }
    }

    void add_to_buffer(juce::OSCMessage message)
    {
        juce::String address = message.getAddressPattern().toString();

        if (!address.compare("/ch/01/mix/fader"))
        {
            float val = message[0].getFloat32();
            juce::String msg_str = address;

            channels[0].fader.set_value(val);

            DBG("ch01 = " << channels[0].fader.get_value() << " dB");
        }
        else
        {
            int num_args = message.size();
            juce::String msg_str = address;

            for (int i = 0; i < num_args; i++)
            {
                msg_str += " " + arg_to_str(message[i]);
            }

            DBG(msg_str);
        }
    }
};