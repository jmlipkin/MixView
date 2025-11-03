// Simple X32 connection thread helper
#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>
// #include <chrono>
#include "Subscriber.h"
#include "Listener.h"

class X32Connect : public juce::Thread
{
public:
    X32Connect() : juce::Thread("X32Connect"), m_timeout(0) { Xip = new juce::IPAddress("127.0.0.1"); }
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
        // juce::DatagramSocket X_socket;
        int Xport = 10023;

    public:
        // juce::OSCSender sender;
        Subscriber sender;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(X32Connect)
};