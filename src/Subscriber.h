#pragma once

#include "Macros.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>
// #include <chrono>

class Subscriber : public juce::OSCSender, public juce::Thread
{
private:
    juce::IPAddress *Xip;
    int Xport = 10023;

    const juce::OSCMessage msg_x32_resub{juce::String("/xremote"), juce::String("on")};
    const juce::OSCMessage msg_x32_getinfo{juce::String("/info")};

    int m_timeout = 2000;

    void resub_to_X32() { this->send(msg_x32_resub); }

    void run() override {     
        while (!threadShouldExit())
        {
            resub_to_X32();
            DBG("X32 resub");
            juce::Thread::sleep(m_timeout);
        }
        DBG("Subscriber thread ending...");
        disconnect();
    }

public:
    Subscriber() : juce::Thread("Subscriber") {}

    void get_info_X32() { this->send(msg_x32_getinfo); }

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    juce::String get_Xip() const { return Xip->toString(); }
    int get_Xport() const { return Xport; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscriber)
};