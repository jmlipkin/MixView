// Simple X32 connection thread helper
#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>
// #include <chrono>
#include "Subscriber.h"
#include "Receiver.h"

class X32Connect : public juce::Component, public juce::Thread
{
public:
    X32Connect();
    ~X32Connect() override { stopThread(1000); free(Xip); sender.disconnect(); }

    // Attempt to connect OSC send/receive to stored IP address/port
    bool connect();
    // Attempt to connect OSC send/receive to specified address/port
    bool connect(juce::String ip, int port);

    void run() override;

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    void set_Xip(std::string ip);
    void set_Xport(int port);

    juce::String get_Xip() const { return Xip->toString(); }
    int get_Xport() const { return Xport; }

    private:
        int m_timeout;
        juce::IPAddress *Xip;
        int Xport = 10023;

        juce::IPAddress *this_ip;
        int this_port = 12345;

        juce::DatagramSocket this_socket;

    public:
        // juce::OSCSender sender;
        Subscriber sender;
        Receiver receiver;

        juce::Label send_label;
        juce::Label rec_label;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(X32Connect)
};