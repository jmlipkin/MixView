#include "Subscriber.h"

Subscriber::Subscriber() : juce::Thread("Subscriber") {}

Subscriber::~Subscriber() {
    disconnect();
    m_connected = false;
}

void Subscriber::check_if_connected() {
    std::chrono::duration<double, std::milli> duration_ms;

    if (m_port == PORT_TMIX)
        duration_ms = std::chrono::high_resolution_clock::now() - tmix->last_thump_time.load();
    else if (m_port == PORT_X32)
        duration_ms = std::chrono::high_resolution_clock::now() - mp->last_thump_time.load();

    double dt_ms = duration_ms.count();

    bool changed = m_connected;
    if (dt_ms < 1000) {
        m_connected = true;
    } else {
        m_connected = false;
    }
    changed ^= m_connected;

    if (changed) {
        state_broadcaster.sendChangeMessage();
    }
}

void Subscriber::run() {
    if (m_port == PORT_X32)
        get_info_X32();
    else if (m_port == PORT_TMIX) 
        get_info_tmix();

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

const juce::OSCMessage msg_x32_getinfo{juce::String("/info")};
const juce::OSCMessage msg_tmix_getinfo{juce::String("/theatremix")};

void Subscriber::get_info_X32() {
    if (m_port == PORT_X32) {
        this->send(msg_x32_getinfo);
    }
}

void Subscriber::get_info_tmix() {
    if (m_port == PORT_TMIX) {
        this->send(msg_tmix_getinfo);
    }
}

const juce::OSCMessage msg_x32_resub{juce::String("/xremote"), juce::String("on")};
const juce::OSCMessage msg_tmix_resub{juce::String("/subscribe")};

void Subscriber::resub_to_x32() {
    this->send(msg_x32_resub);
    this->send("/info");
}

void Subscriber::resub_to_tmix() {
    this->send(msg_tmix_resub);
    this->send("/thump");
}