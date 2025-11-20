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
        bool receiver{DISCONNECTED};
    };

   private:
    Subscriber sender_X32;
    Subscriber sender_tmix;
    Receiver receiver;

    juce::DatagramSocket* socket_this;
    Connected_State state;

    juce::IPAddress* ip_X32;
    juce::IPAddress* ip_tmix;
    juce::IPAddress* ip_this;

    static const int PORT_X32{10023};
    static const int PORT_TMIX{32000};

    int port_this{12345};

    int m_timeout;

   public:
    OSCConnect();
    ~OSCConnect() override;

    // Attempt to connect OSC send/receive to stored IP address/port
    Connected_State connect();
    // Attempt to connect OSC send/receive to specified address/port
    bool connect(juce::String ip, int port);

    bool connect_to_tmix();

    void synchronize_with_X32();

    void open();
    void close(int timeout_milliseconds);

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

    // Warning: undefined behavior if called while connected.
    void set_ip_X32(std::string ip);
    // Warning: undefined behavior if called while connected.
    void set_ip_tmix(std::string ip);
    // Warning: undefined behavior if called while connected.
    void set_ip_this(std::string ip);

    juce::String get_ip_X32() const { return ip_X32->toString(); }
    juce::String get_ip_tmix() const { return ip_tmix->toString(); }
    juce::String get_ip_this() const { return ip_this->toString(); }
    int get_port_X32() const { return PORT_X32; }
    int get_port_tmix() const { return PORT_TMIX; }

    Connected_State get_connection_status() { return state; }

    MessageProcessor* get_message_processor() { return receiver.get_message_processor(); }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCConnect)
};

class IPAddressBox : public juce::Label {
   public:
    IPAddressBox() {
        setEditable(true);
        onTextChange = nullptr;  // We don't validate on every keystroke
    }

    // Sets a new value programmatically and stores it as valid
    void setIPAddress(const juce::String& newAddress) {
        storedString = newAddress;
        setText(newAddress, juce::dontSendNotification);
    }

    juce::String getIPAddress() const {
        return storedString;
    }

    bool isChanged() { return m_isChanged; }
    void set_isChanged(bool new_state) { m_isChanged = new_state; }

   protected:

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override { tryCommit(); }

   private:
    static const int MIN_NUM_CHARS_IPV4{7};
    static const int MAX_NUM_CHARS_IPV4{15};
    static inline const juce::String ALLOWED_CHARS{"1234567890."};
    bool m_isChanged;
    // Your future IPv4 validator — you will implement this
    juce::String filter_text(const juce::String& str);
    const juce::String& get_allowed_chars();

    void tryCommit() {
        auto text = filter_text(getText(true));

        storedString = text;
        setText(storedString, juce::dontSendNotification);
    }

    juce::String storedString;
};