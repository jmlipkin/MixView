#pragma once

#include "Macros.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

class Receiver : public juce::OSCReceiver, juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
private:
    juce::String arg_to_str(const juce::OSCArgument &arg);

public:
    Receiver() {}

    bool connect_and_start(juce::DatagramSocket &socket);

    void oscMessageReceived(const juce::OSCMessage &message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Receiver)
};