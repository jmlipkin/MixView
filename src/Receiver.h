#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include "MessageProcessor.h"

class Receiver : public juce::OSCReceiver, public juce::OSCReceiver::Listener<juce::OSCReceiver::RealtimeCallback> {
   public:
    // Sets stored MessageProcessor to the referenced processor. 
    Receiver(MessageProcessor& processor);

    // Runs close().
    ~Receiver() override;

    // Adds itself as an OSCReceiver listener.
    void open();

    // Removes itself as an OSCReceiver listener and disconnects from
    // the currently bound socket.
    void close();

   private:
    MessageProcessor& mp;

    // Adds the received message to the stored MessageProcessor's buffer.
    void oscMessageReceived(const juce::OSCMessage& message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Receiver)
};