#pragma once

#include "ChannelStrip.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

class MessageProcessor
{
private:
    const size_t NUM_IN_CH = 32;
    const size_t NUM_AUXINS = 8;
    const size_t NUM_BUSES = 16;
    const size_t NUM_MATRICES = 6;
    const size_t NUM_DCAS = 8;

    InputChannelStrip *channels;
    InputChannelStrip *auxins;
    InputChannelStrip *buses;
    ChannelStrip *matrices;
    ChannelStrip *dcas;

    juce::Array<juce::OSCMessage> msg_buffer[2048];

public:
    MessageProcessor()
    {
        channels = new InputChannelStrip[NUM_IN_CH];
        auxins = new InputChannelStrip[NUM_AUXINS];
        buses = new InputChannelStrip[NUM_BUSES];
        matrices = new ChannelStrip[NUM_MATRICES];
        dcas = new DCAChannelStrip[NUM_DCAS];

        for (size_t i = 0; i < NUM_IN_CH; i++)
        {
            channels[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_AUXINS; i++)
        {
            auxins[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_BUSES; i++)
        {
            buses[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_MATRICES; i++)
        {
            matrices[i].set_num_and_ap(i);
        }
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
        size_t idx = 100;
        switch (find_channel_type(message))
        {
        case ChannelStrip::CH_TYPE::IN_CH:
        {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::IN_CH);
            float val = message[0].getFloat32();
            channels[idx].set_fader_value(val);
            DBG("Ch " << channels[idx].get_num_index() + 1 << " set to " << channels[idx].get_fader_value() << " dB");
            break;
        }
        case ChannelStrip::CH_TYPE::AUX_IN:
        {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::AUX_IN);
            print_unprocessed_string(message);
            break;
        }
        case ChannelStrip::CH_TYPE::BUS:
        {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::BUS);
            print_unprocessed_string(message);
            break;
        }
        case ChannelStrip::CH_TYPE::DCA:
        {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::DCA);
            print_unprocessed_string(message);
            break;
        }
        case ChannelStrip::CH_TYPE::MATRIX:
        {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::MATRIX);
            print_unprocessed_string(message);
            break;
        }
        case ChannelStrip::CH_TYPE::INVALID:
        {
            print_unprocessed_string(message);
            break;
        }
        default:
            print_unprocessed_string(message);
            break;
        }
    }

    ChannelStrip::CH_TYPE find_channel_type(juce::OSCMessage &message)
    {

        juce::String address = message.getAddressPattern().toString();

        for (size_t i = 0; i < sizeof(ChannelStrip::channel_type) / sizeof(ChannelStrip::channel_type[0]); i++)
        {
            if (address.startsWith(ChannelStrip::channel_type[i]))
            {
                address = address.substring(ChannelStrip::channel_type[i].length());
                message.setAddressPattern(address);
                return static_cast<ChannelStrip::CH_TYPE>(i);
            }
        }

        return ChannelStrip::INVALID;
    }

    size_t find_channel_idx(juce::OSCMessage &message, ChannelStrip::CH_TYPE type)
    {

        juce::String address = message.getAddressPattern().toString();

        switch (type)
        {
        case ChannelStrip::IN_CH:
            for (size_t i = 0; i < NUM_IN_CH; i++)
            {
                if (address.startsWith(channels[i].get_num_ap().toString()))
                {
                    address = address.substring(channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
            break;
        case ChannelStrip::CH_TYPE::AUX_IN:
        {
            for (size_t i = 0; i < NUM_AUXINS; i++)
            {
                if (address.startsWith(channels[i].get_num_ap().toString()))
                {
                    address = address.substring(channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::BUS:
        {
            for (size_t i = 0; i < NUM_BUSES; i++)
            {
                if (address.startsWith(channels[i].get_num_ap().toString()))
                {
                    address = address.substring(channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }

        }
        case ChannelStrip::CH_TYPE::DCA:
        {
            for (size_t i = 0; i < NUM_DCAS; i++)
            {
                if (address.startsWith(channels[i].get_num_ap().toString()))
                {
                    address = address.substring(channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::MATRIX:
        {
            for (size_t i = 0; i < NUM_MATRICES; i++)
            {
                if (address.startsWith(channels[i].get_num_ap().toString()))
                {
                    address = address.substring(channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::INVALID:
            return 100;
        default:
            return 100;
        }
    }

    void print_unprocessed_string(juce::OSCMessage &message)
    {
        juce::String address = message.getAddressPattern().toString();
        int num_args = message.size();
        juce::String msg_str = address;

        for (int i = 0; i < num_args; i++)
        {
            msg_str += " " + arg_to_str(message[i]);
        }

        DBG(msg_str);
    }
};