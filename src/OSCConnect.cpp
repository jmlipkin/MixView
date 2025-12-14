#include "OSCConnect.h"

OSCConnect::OSCConnect(MessageProcessor& processor, juce::IPAddress host, juce::IPAddress console, juce::IPAddress tmix) : m_ip_host(host), m_ip_console(console), m_ip_tmix(tmix), m_timeout(0), m_mp(processor) {

    m_sender_tmix.set_port(PORT_TMIX);
    m_sender_X32.set_port(PORT_CONSOLE);

    m_socket_console = new juce::DatagramSocket();
    m_socket_console->setEnablePortReuse(true);
    m_socket_tmix = new juce::DatagramSocket();
}

OSCConnect::~OSCConnect() {
    m_socket_console->shutdown();
    m_socket_tmix->shutdown();

    delete m_socket_console;
    m_socket_console = nullptr;

    delete m_socket_tmix;
    m_socket_tmix = nullptr;
}

void OSCConnect::open() {
    if (m_socket_console->getBoundPort() != -1) {
        bind_socket_X32();
        m_sender_X32.startThread();
        DBG("X32 Send thread started" << DBG_STR);
        m_receiver_X32.open();
        DBG("X32 Receiver opened" << DBG_STR);
        synchronize_with_X32();
    }
    if(m_socket_tmix->getBoundPort() != -1) {
        bind_socket_tmix();
        m_sender_tmix.startThread();
        DBG("TMix Send thread started" << DBG_STR);
        m_receiver_tmix.open();
        DBG("TMix Receiver opened:" << DBG_STR);
    }
}

void OSCConnect::close(int timeout_milliseconds) {
    m_sender_X32.stopThread(timeout_milliseconds);
    DBG("X32 Send thread stopped" << DBG_STR);
    m_receiver_X32.close();
    DBG("X32 Receiver closed" << DBG_STR);
    m_sender_tmix.stopThread(timeout_milliseconds);
    DBG("TMix Send thread stopped" << DBG_STR);
    m_receiver_tmix.close();
    DBG("TMix Receiver closed" << DBG_STR);
}

// TODO: ERROR HANDLING
OSCConnect::Connected_State OSCConnect::bind_all_ports() {
    m_socket_tmix->bindToPort(0, m_ip_host.toString());
    m_socket_console->bindToPort(0, m_ip_host.toString());

    bind_socket_tmix();
    bind_socket_X32();

    return m_state;
}

void OSCConnect::bind_socket_X32() {
    if(m_socket_console->getBoundPort() != -1)
    {
        m_state.X32 = m_sender_X32.connectToSocket(*m_socket_console, m_ip_console.toString(), PORT_CONSOLE);
        // DBG("Create X32 socket send: " << (state.X32 ? "success" : "fail") << DBG_STR);
        if (m_state.X32) {
            m_state.X32 &= m_receiver_X32.connectToSocket(*m_socket_console);
            // DBG("Create X32 socket receive: " << (state.X32 ? "success" : "fail") << DBG_STR);
        }
        if (m_state.X32) {
            DBG("X32 sockets bound successfully." << DBG_STR);
        }
    }
}

void OSCConnect::bind_socket_tmix() {
    if(m_socket_tmix->getBoundPort() != -1)
    {
        m_state.TMix = m_sender_tmix.connectToSocket(*m_socket_tmix, m_ip_tmix.toString(), PORT_TMIX);
        // DBG("Create TMix socket send: " << (state.TMix ? "success" : "fail") << DBG_STR);
        if (m_state.TMix) {
            m_state.TMix &= m_receiver_tmix.connectToSocket(*m_socket_tmix);
            // DBG("Create TMix socket receive: " << (state.TMix ? "success" : "fail") << DBG_STR);
        }
        if (m_state.TMix) {
            DBG("TMix sockets bound successfully." << DBG_STR);
        }
    }
}

void OSCConnect::set_ip_host(juce::IPAddress addr) {
    m_ip_host = addr;
    DBG("Host IP set to " << addr.toString() << DBG_STR);
}

void OSCConnect::set_ip_console(juce::IPAddress addr) {
    m_ip_console = addr;
    DBG("Console IP set to " << addr.toString() << DBG_STR);
}

void OSCConnect::set_ip_tmix(juce::IPAddress addr) {
    m_ip_tmix = addr;
    DBG("TMix IP set to " << addr.toString() << DBG_STR);
}

void OSCConnect::free_socket_X32() {
    m_sender_X32.disconnect();
    m_receiver_X32.disconnect();
}

void OSCConnect::free_socket_tmix() {
    m_sender_tmix.disconnect();
    m_receiver_tmix.disconnect();
}

void OSCConnect::synchronize_with_X32() {

    for (int ch = 1; ch <= 32; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);

        m_sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/config/name"));
        m_sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/config/color"));
        m_sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/mix/on"));
        m_sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/mix/fader"));
        m_sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 8; ch++) {
        m_sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/name"));
        m_sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/color"));
        m_sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/on"));
        m_sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/fader"));
        m_sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/grp/dca"));

        m_sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/name"));
        m_sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/color"));
        m_sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/on"));
        m_sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/fader"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 16; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);

        m_sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/config/name"));
        m_sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/config/color"));
        m_sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/mix/on"));
        m_sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/mix/fader"));
        m_sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }
    for (int ch = 1; ch <= 6; ch++) {
        m_sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/name"));
        m_sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/color"));
        m_sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/on"));
        m_sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/fader"));
        juce::Thread::sleep(1);
    }

    m_sender_X32.send(juce::OSCMessage("/main/st/config/name"));
    m_sender_X32.send(juce::OSCMessage("/main/st/config/color"));
    m_sender_X32.send(juce::OSCMessage("/main/st/mix/on"));
    m_sender_X32.send(juce::OSCMessage("/main/st/mix/fader"));

    DBG("X32 Synchronization complete" << DBG_STR);
}