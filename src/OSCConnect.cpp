#include "OSCConnect.h"

OSCConnect::OSCConnect(MessageProcessor& processor, juce::IPAddress host, juce::IPAddress console, juce::IPAddress tmix) : mp(processor), ip_host(host), ip_console(console), ip_tmix(tmix), m_timeout(0) {
    sender_tmix.set_port(PORT_TMIX);
    sender_X32.set_port(PORT_CONSOLE);

    socket_console = new juce::DatagramSocket();
    socket_console->setEnablePortReuse(true);
    socket_tmix = new juce::DatagramSocket();
}

OSCConnect::~OSCConnect() {
    socket_console->shutdown();
    socket_tmix->shutdown();

    delete socket_console;
    socket_console = nullptr;

    delete socket_tmix;
    socket_tmix = nullptr;
}

void OSCConnect::open() {
    if (socket_console->getBoundPort() != -1) {
        sender_X32.startThread();
        DBG("X32 Send thread started" << DBG_STR);
        receiver_X32.open();
        DBG("X32 Receiver opened" << DBG_STR);
        synchronize_with_X32();
    }
    if(socket_tmix->getBoundPort() != -1) {
        sender_tmix.startThread();
        DBG("TMix Send thread started" << DBG_STR);
        receiver_tmix.open();
        DBG("TMix Receiver opened:" << DBG_STR);
    }
}

void OSCConnect::close(int timeout_milliseconds) {
    sender_X32.stopThread(timeout_milliseconds);
    DBG("X32 Send thread stopped" << DBG_STR);
    receiver_X32.close();
    DBG("X32 Receiver closed" << DBG_STR);
    sender_tmix.stopThread(timeout_milliseconds);
    DBG("TMix Send thread stopped" << DBG_STR);
    receiver_tmix.close();
    DBG("TMix Receiver closed" << DBG_STR);
}

void OSCConnect::synchronize_with_X32() {
    sender_X32.get_info_X32();

    for (int ch = 1; ch <= 32; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);

        sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/config/name"));
        sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/config/color"));
        sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/mix/on"));
        sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/mix/fader"));
        sender_X32.send(juce::OSCMessage("/ch/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 8; ch++) {
        sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/name"));
        sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/color"));
        sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/on"));
        sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/fader"));
        sender_X32.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/grp/dca"));

        sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/name"));
        sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/color"));
        sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/on"));
        sender_X32.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/fader"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 16; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);

        sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/config/name"));
        sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/config/color"));
        sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/mix/on"));
        sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/mix/fader"));
        sender_X32.send(juce::OSCMessage("/bus/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }
    for (int ch = 1; ch <= 6; ch++) {
        sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/name"));
        sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/color"));
        sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/on"));
        sender_X32.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/fader"));
        juce::Thread::sleep(1);
    }

    sender_X32.send(juce::OSCMessage("/main/st/config/name"));
    sender_X32.send(juce::OSCMessage("/main/st/config/color"));
    sender_X32.send(juce::OSCMessage("/main/st/mix/on"));
    sender_X32.send(juce::OSCMessage("/main/st/mix/fader"));

    DBG("X32 Synchronization complete" << DBG_STR);
}

void OSCConnect::bind_socket_X32() {
    if(socket_console->getBoundPort() != -1)
    {
        state.X32 = sender_X32.connectToSocket(*socket_console, ip_console.toString(), PORT_CONSOLE);
        // DBG("Create X32 socket send: " << (state.X32 ? "success" : "fail") << DBG_STR);
        if (state.X32) {
            state.X32 &= receiver_X32.connectToSocket(*socket_console);
            // DBG("Create X32 socket receive: " << (state.X32 ? "success" : "fail") << DBG_STR);
        }
        if (state.X32) {
            DBG("X32 sockets bound successfully." << DBG_STR);
        }
    }
}

void OSCConnect::bind_socket_tmix() {
    if(socket_tmix->getBoundPort() != -1)
    {
        state.TMix = sender_tmix.connectToSocket(*socket_tmix, ip_tmix.toString(), PORT_TMIX);
        // DBG("Create TMix socket send: " << (state.TMix ? "success" : "fail") << DBG_STR);
        if (state.TMix) {
            state.TMix &= receiver_tmix.connectToSocket(*socket_tmix);
            // DBG("Create TMix socket receive: " << (state.TMix ? "success" : "fail") << DBG_STR);
        }
        if (state.TMix) {
            DBG("TMix sockets bound successfully." << DBG_STR);
        }
    }
}

// TODO: ERROR HANDLING
OSCConnect::Connected_State OSCConnect::bind_all_ports() {
    socket_tmix->bindToPort(0, ip_host.toString());
    socket_console->bindToPort(0, ip_host.toString());

    bind_socket_tmix();
    bind_socket_X32();

    return state;
}

void OSCConnect::set_ip_host(juce::IPAddress addr) {
    ip_host = addr;
    DBG("Host IP set to " << addr.toString() << DBG_STR);
}

void OSCConnect::set_ip_console(juce::IPAddress addr) {
    ip_console = addr;
    DBG("Console IP set to " << addr.toString() << DBG_STR);
}

void OSCConnect::set_ip_tmix(juce::IPAddress addr) {
    ip_tmix = addr;
    DBG("TMix IP set to " << addr.toString() << DBG_STR);
}