#include "Receiver.h"

bool Receiver::connect_and_start(juce::DatagramSocket &socket) {
    bool success = connectToSocket(socket);
    if(success)
    {
        addListener(this);
    }

    return success;
}

void Receiver::oscMessageReceived(const juce::OSCMessage &message) {
    int num_args = message.size();
    juce::String msg_str = message.getAddressPattern().toString();

    for (int i = 0; i < num_args; i++)
    {
        msg_str += " " + arg_to_str(message[i]);
    }

    DBG(msg_str << DBG_STR);
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