#pragma once

#include "Macros.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

class Receiver : public juce::OSCReceiver::Listener<juce::OSCReceiver::RealtimeCallback>,
                  public juce::Thread
{
private:
    juce::String arg_to_str(const juce::OSCArgument &arg);
    juce::OSCReceiver receiver;
    juce::DatagramSocket* m_socket;
    juce::CriticalSection socketLock;

public:
    Receiver() : juce::Thread("Receiver") {}

    ~Receiver() override {
        stopThread(1000);
        receiver.removeListener(this);
        receiver.disconnect();
    }

    bool connectToSocket(juce::DatagramSocket& socket) {
        const juce::ScopedLock sl(socketLock);
        m_socket = &socket;
        return receiver.connectToSocket(socket);
    }

    void disconnect() {
        const juce::ScopedLock sl(socketLock);
        receiver.disconnect();
    }

    void run() override;
    void oscMessageReceived(const juce::OSCMessage& message) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Receiver)
};