#include "Receiver.h"

void Receiver::oscMessageReceived(const juce::OSCMessage &message) {
    mp.add_to_buffer(message);
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