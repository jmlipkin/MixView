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
    int PORT_X32 = 10023;
    int PORT_TMIX = 32000;

    const juce::OSCMessage msg_x32_resub{juce::String("/xremote"), juce::String("on")};
    const juce::OSCMessage msg_x32_getinfo{juce::String("/info")};

    const juce::OSCMessage msg_tmix_resub{juce::String("/subscribe")};

    int m_timeout = 2000;

    void resub_to_x32() { this->send(msg_x32_resub); }
    void resub_to_tmix() { this->send(msg_tmix_resub); }

    void run() override {     
        while (!threadShouldExit())
        {
            resub_to_x32();
            juce::Thread::sleep(m_timeout);
        }
        DBG("Subscriber thread ending...");
        disconnect();
    }

public:
    Subscriber() : juce::Thread("Subscriber") {}
    ~Subscriber() override { disconnect(); }

    void get_info_X32() { this->send(msg_x32_getinfo); }

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    juce::String get_Xip() const { return Xip->toString(); }
    int get_port_x32 () const { return PORT_X32; }
    int get_port_tmix () const { return PORT_TMIX; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscriber)
};