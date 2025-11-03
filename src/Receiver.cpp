#include "Receiver.h"

void Receiver::oscMessageReceived(const juce::OSCMessage &message) {
    juce::String address = message.getAddressPattern().toString();
    if (!address.compare("/ch/01/mix/fader"))
    {
        int num_args = message.size();
        float val = message[0].getFloat32();
        juce::String msg_str = address;

        // for (int i = 0; i < num_args; i++)
        // {
        //     msg_str += " " + arg_to_str(message[i]);
        // }

        DBG("ch01 = " << ch1.get_value() << " dB");
    }
    else {
        int num_args = message.size();
        juce::String msg_str = address;

        for (int i = 0; i < num_args; i++)
        {
            msg_str += " " + arg_to_str(message[i]);
        }

        DBG(msg_str);
    }
}

juce::String Receiver::arg_to_str(const juce::OSCArgument &arg) {
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