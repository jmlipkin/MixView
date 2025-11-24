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
    juce::Label send_label;
    juce::Label rec_label;

    enum STATUS_CONNECT {
        DISCONNECTED,
        CONNECTED
    };
    struct Connected_State {
        bool X32{DISCONNECTED};
        bool TMix{DISCONNECTED};
    };

   private:
    MessageProcessor& mp;

    Subscriber sender_X32;
    Subscriber sender_tmix;
    Receiver receiver_X32{mp};
    Receiver receiver_tmix{mp};

    juce::DatagramSocket* socket_console;
    juce::DatagramSocket* socket_tmix;

    Connected_State state;

    juce::IPAddress ip_host;
    juce::IPAddress ip_console;
    juce::IPAddress ip_tmix;

    static const int PORT_CONSOLE{10023};
    static const int PORT_TMIX{32000};

    int port_this{12345};

    int m_timeout;

   public:
    OSCConnect(MessageProcessor& processor, juce::IPAddress host, juce::IPAddress console, juce::IPAddress tmix);
    ~OSCConnect() override;

    // Attempt to connect OSC send/receive to stored IP address/port
    Connected_State bind_all_ports();
    // Attempt to connect OSC send/receive to specified address/port
    bool connect(juce::String ip, int port);

    void bind_socket_X32();
    void bind_socket_tmix();

    void free_socket_X32();
    void free_socket_tmix();

    void synchronize_with_X32();

    void open();
    void close(int timeout_milliseconds);

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    // Warning: undefined behavior if called while connected.
    void set_ip_console(juce::IPAddress addr);
    // Warning: undefined behavior if called while connected.
    void set_ip_tmix(juce::IPAddress addr);
    // Warning: undefined behavior if called while connected.
    void set_ip_host(juce::IPAddress addr);

    int get_port_console() const { return PORT_CONSOLE; }
    int get_port_tmix() const { return PORT_TMIX; }

    Connected_State get_connection_status() { return state; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCConnect)
};