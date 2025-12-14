// Simple X32 connection thread helper
#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

#include "Receiver.h"
#include "Subscriber.h"

class OSCConnect : public juce::Component {
   public:
    enum STATUS_CONNECT {
        DISCONNECTED,
        CONNECTED
    };
    struct Connected_State {
        bool X32{DISCONNECTED};
        bool TMix{DISCONNECTED};
    };

   public:
    OSCConnect(MessageProcessor& processor, juce::IPAddress host, juce::IPAddress console, juce::IPAddress tmix);
    ~OSCConnect() override;

    // Main point of entry for starting the Subscribers and Receivers.
    // Called when user wants to start listening to updates.
    void open();

    // Main exit point on disconnect. Stops send and receive threads.
    void close(int timeout_milliseconds);

    // Attempt to connect OSC send/receive to stored IP address/port
    Connected_State bind_all_ports();

    void bind_socket_X32();
    void bind_socket_tmix();

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    // Warning: undefined behavior if called while connected.
    void set_ip_host(juce::IPAddress addr);
    // Warning: undefined behavior if called while connected.
    void set_ip_console(juce::IPAddress addr);
    // Warning: undefined behavior if called while connected.
    void set_ip_tmix(juce::IPAddress addr);

    Subscriber* get_sub_x32() { return &m_sender_X32; }
    Subscriber* get_sub_tmix() { return &m_sender_tmix; }
    Receiver* get_rec_x32() { return &m_receiver_X32; }
    Receiver* get_rec_tmix() { return &m_receiver_tmix; }

    Connected_State get_connection_status() { return m_state; }

   private:
    // Disconnects send/receive sockets
    void free_socket_X32();

    // Disconnects send/receive sockets
    void free_socket_tmix();

    // Sends get request for all relevant parameters from the console
    void synchronize_with_X32();

   private:
    juce::DatagramSocket* m_socket_console;
    juce::DatagramSocket* m_socket_tmix;

    juce::IPAddress m_ip_host;
    juce::IPAddress m_ip_console;
    juce::IPAddress m_ip_tmix;

    Connected_State m_state;

    static const int PORT_CONSOLE{10023};
    static const int PORT_TMIX{32000};

    int m_port_this{12345};

    int m_timeout;

    MessageProcessor& m_mp;

    Subscriber m_sender_X32;
    Subscriber m_sender_tmix;
    Receiver m_receiver_X32{m_mp};
    Receiver m_receiver_tmix{m_mp};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCConnect)
};