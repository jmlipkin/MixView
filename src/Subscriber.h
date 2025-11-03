#pragma once

#include "Macros.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>
// #include <chrono>

class Subscriber : public juce::OSCSender
{
private:
    juce::IPAddress *Xip;
    int Xport = 10023;

    const juce::OSCMessage msg_x32_resub{juce::String("/xremote"), juce::String("on")};
    const juce::OSCMessage msg_x32_getinfo{juce::String("/info")};

    int m_timeout;

public:
    Subscriber() {}

    void resub_to_X32() { this->send(msg_x32_resub); }
    void get_info_X32() { this->send(msg_x32_getinfo); }

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    juce::String get_Xip() const { return Xip->toString(); }
    int get_Xport() const { return Xport; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscriber)
};