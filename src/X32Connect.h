// Simple X32 connection thread helper
#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>
// #include <chrono>

class X32Connect : public juce::Thread, juce::OSCSender
{
public:
    X32Connect() : Thread("X32Connect"), m_timeout(0) { Xip = new juce::IPAddress("127.0.0.1"); }
    ~X32Connect() override { stopThread(1000); free(Xip); }

    // bool connect();

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
        juce::OSCSender sender;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(X32Connect)
};