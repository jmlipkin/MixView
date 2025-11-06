#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include "Macros.h"
#include "MessageProcessor.h"

class Receiver : public juce::OSCReceiver, public juce::OSCReceiver::Listener<juce::OSCReceiver::RealtimeCallback> {
   private:
    juce::String arg_to_str(const juce::OSCArgument& arg);
    juce::DatagramSocket* m_socket;
    MessageProcessor mp;

   public:
    Receiver() : mp() {}
    ~Receiver() override {
        close();
    }

    void setSocket(juce::DatagramSocket* socket) { m_socket = socket; }

    void open() { addListener(this); }
    void close() {
        removeListener(this);
        disconnect();
    }

    void oscMessageReceived(const juce::OSCMessage& message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Receiver)
};