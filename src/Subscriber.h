#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include "Macros.h"
// #include <chrono>

class Subscriber : public juce::OSCSender, public juce::Thread {
   public:
    juce::ChangeBroadcaster state_broadcaster;

   private:
    static const int PORT_X32 = 10023;
    static const int PORT_TMIX = 32000;

    int m_port;

    const juce::OSCMessage msg_x32_resub{juce::String("/xremote"), juce::String("on")};
    const juce::OSCMessage msg_tmix_resub{juce::String("/subscribe")};

    const juce::OSCMessage msg_x32_getinfo{juce::String("/info")};
    const juce::OSCMessage msg_tmix_getinfo{juce::String("/theatremix")};

    int m_timeout = 200;
    bool connected = false;

    TMixProcessor* tmix = nullptr;
    MessageProcessor* mp = nullptr;

    void resub_to_x32() {
        this->send(msg_x32_resub);
        this->send("/info");
    }

    void resub_to_tmix() {
        this->send(msg_tmix_resub);
        this->send("/thump");
    }

    void run() override {
        if (m_port == PORT_X32)
            this->send(msg_x32_getinfo);
        else if (m_port == PORT_TMIX)
            this->send(msg_tmix_resub);

        while (!threadShouldExit()) {
            if (m_port == PORT_X32)
                resub_to_x32();
            else if (m_port == PORT_TMIX) {
                resub_to_tmix();
            }
            check_if_connected();
            juce::Thread::sleep(m_timeout);
        }
        DBG("Subscriber thread ending...");
        disconnect();
    }

   public:
    Subscriber() : juce::Thread("Subscriber") {}
    ~Subscriber() override {
        disconnect();
        connected = false;
    }

    // Must be called in container's constructor
    void set_port(int port) { m_port = port; }
    void set_tmix(TMixProcessor* tmix_loc) { tmix = tmix_loc; }
    void set_mp(MessageProcessor* mp_loc) { mp = mp_loc; }

    void get_info_X32() {
        if (m_port == PORT_X32) {
            this->send(msg_x32_getinfo);
        }
    }
    void get_info_tmix() {
        if (m_port == PORT_TMIX) {
            this->send(msg_tmix_getinfo);
        }
    }

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    void check_if_connected() {
        std::chrono::duration<double, std::milli> duration_ms;

        if (m_port == PORT_TMIX)
            duration_ms = std::chrono::high_resolution_clock::now() - tmix->last_thump_time.load();
        else if (m_port == PORT_X32)
            duration_ms = std::chrono::high_resolution_clock::now() - mp->last_thump_time.load();

        double dt_ms = duration_ms.count();

        bool changed = connected;
        if (dt_ms < 1000) {
            connected = true;
        } else {
            connected = false;
        }
        changed ^= connected;

        if (changed) {
            state_broadcaster.sendChangeMessage();
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Subscriber)
};