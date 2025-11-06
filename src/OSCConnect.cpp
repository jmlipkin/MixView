#include "OSCConnect.h"

#include <juce_core/juce_core.h>

#define IP_X32 "127.0.0.1"

OSCConnect::OSCConnect() : m_timeout(0) {
    sender_tmix.set_port(PORT_TMIX);
    sender_X32.set_port(PORT_X32);

    ip_X32 = new juce::IPAddress();
    ip_tmix = new juce::IPAddress();
    ip_this = new juce::IPAddress();

    socket_this = new juce::DatagramSocket();
    socket_this->setEnablePortReuse(true);
}

OSCConnect::~OSCConnect() {
    if (ip_X32 != nullptr) delete ip_X32;
    if (ip_tmix != nullptr) delete ip_tmix;
    if (ip_this != nullptr) delete ip_this;

    ip_this = nullptr;
    ip_tmix = nullptr;
    ip_X32 = nullptr;

    socket_this->shutdown();

    delete socket_this;
    socket_this = nullptr;
}

void OSCConnect::open() {
    sender_X32.startThread();
    sender_tmix.startThread();
    receiver.open();
    synchronize_with_X32();
}

void OSCConnect::close(int timeout_milliseconds) {
    sender_X32.stopThread(timeout_milliseconds);
    sender_tmix.stopThread(timeout_milliseconds);
    receiver.close();

    socket_this->shutdown();
    delete socket_this;
    socket_this = new juce::DatagramSocket();
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
}

bool OSCConnect::connect_to_tmix() {
    bool success = sender_tmix.connectToSocket(*socket_this, ip_tmix->toString(), PORT_TMIX);
    if (success) {
        DBG("Connected successfully to TheatreMix");
    }
    return success;
}

OSCConnect::Connected_State OSCConnect::connect() {
    bool success = socket_this->bindToPort(port_this, ip_this->toString());
    if (success) {
        state.X32 = sender_X32.connectToSocket(*socket_this, ip_X32->toString(), PORT_X32);
        success &= state.X32;
    }
    if (success) {
        state.receiver = receiver.connectToSocket(*socket_this);
        success &= state.receiver;
        receiver.setSocket(socket_this);
    }
    if (success) {
        DBG("Connected successfully to X32");
        synchronize_with_X32();
    }
    state.TMix = connect_to_tmix();

    return state;
}

void OSCConnect::set_ip_X32(std::string ip) {
    if (ip_X32 != nullptr) {
        delete ip_X32;
        ip_X32 = nullptr;
    }
    ip_X32 = new juce::IPAddress(ip);
}

void OSCConnect::set_ip_tmix(std::string ip) {
    if (ip_tmix != nullptr) {
        delete ip_tmix;
        ip_tmix = nullptr;
    }
    ip_tmix = new juce::IPAddress(ip);
}
void OSCConnect::set_ip_this(std::string ip) {
    if (ip_this != nullptr) {
        delete ip_this;
        ip_tmix = nullptr;
    }
    ip_this = new juce::IPAddress(ip);
}