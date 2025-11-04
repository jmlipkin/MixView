#include "ChannelStrip.h"


void ChannelStrip::set_num_and_ap(size_t idx) {
    if(idx + 1 < 10)
        number.ap = "/0" + juce::String(idx + 1);
    else
        number.ap = "/" + juce::String(idx + 1);
    number.index = idx;
}

void DCAChannelStrip::set_num_and_ap(size_t idx) {
    number.ap = "/" + juce::String(idx + 1);
    number.index = idx;
}

void ChannelStrip::print_string_with_ch_id(juce::OSCMessage &message) {

    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();
    juce::String msg_str = address;

    for (int i = 0; i < num_args; i++)
    {
        msg_str += " " + arg_to_str(message[i]);
    }

    DBG(name.value << " " << number.index + 1 << ": " << msg_str);
}

juce::String ChannelStrip::arg_to_str(const juce::OSCArgument &arg)
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

const std::vector<juce::String> ChannelStrip::channel_type = {"/ch", "/auxin", "/bus", "/mtx", "/dca"};