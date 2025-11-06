#include "OSCConnect.h"

#include <juce_core/juce_core.h>

#define IP_X32 "10.5.136.59"

OSCConnect::OSCConnect() : m_timeout(0)
{
    Xip = new juce::IPAddress(IP_X32);
    this_ip = new juce::IPAddress(IP_X32);
    this_socket = new juce::DatagramSocket();
}

void OSCConnect::run()
{
    sender.startThread();
    receiver.open();
    synchronize_with_X32();
}

void OSCConnect::close(int timeout_milliseconds)
{
    sender.stopThread(timeout_milliseconds);
    receiver.close();

    this_socket->shutdown();
    delete this_socket;
    this_socket = new juce::DatagramSocket();
}

void OSCConnect::synchronize_with_X32() {
    sender.get_info_X32();

    for (int ch = 1; ch <= 32; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);
    
        sender.send(juce::OSCMessage("/ch/" + chNum + "/config/name"));
        sender.send(juce::OSCMessage("/ch/" + chNum + "/config/color"));
        sender.send(juce::OSCMessage("/ch/" + chNum + "/mix/on"));
        sender.send(juce::OSCMessage("/ch/" + chNum + "/mix/fader"));
        sender.send(juce::OSCMessage("/ch/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 8; ch++) {
    
        sender.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/name"));
        sender.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/config/color"));
        sender.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/on"));
        sender.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/mix/fader"));
        sender.send(juce::OSCMessage("/auxin/0" + juce::String(ch) + "/grp/dca"));
        
        sender.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/name"));
        sender.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/config/color"));
        sender.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/on"));
        sender.send(juce::OSCMessage("/dca/" + juce::String(ch) + "/fader"));
        juce::Thread::sleep(1);
    }

    for (int ch = 1; ch <= 16; ch++) {
        juce::String chNum = (ch < 10) ? "0" + juce::String(ch) : juce::String(ch);
    
        sender.send(juce::OSCMessage("/bus/" + chNum + "/config/name"));
        sender.send(juce::OSCMessage("/bus/" + chNum + "/config/color"));
        sender.send(juce::OSCMessage("/bus/" + chNum + "/mix/on"));
        sender.send(juce::OSCMessage("/bus/" + chNum + "/mix/fader"));
        sender.send(juce::OSCMessage("/bus/" + chNum + "/grp/dca"));
        juce::Thread::sleep(1);
    }
    for (int ch = 1; ch <= 6; ch++) {
        sender.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/name"));
        sender.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/config/color"));
        sender.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/on"));
        sender.send(juce::OSCMessage("/mtx/0" + juce::String(ch) + "/mix/fader"));
        juce::Thread::sleep(1);
    }
}

bool OSCConnect::connect()
{
    bool success = this_socket->bindToPort(this_port, this_ip->toString());
    if (success)
        success &= sender.connectToSocket(*this_socket, Xip->toString(), PORT_X32);
    if (success) {
        success &= receiver.connectToSocket(*this_socket);
        receiver.setSocket(this_socket);
    }
    if (success) {
        DBG("Connected successfully");
        synchronize_with_X32();
    }

    return success;
}

bool OSCConnect::connect(juce::String ip, int port)
{
    bool success = this_socket->bindToPort(this_port, this_ip->toString());
    if (success)
        success &= sender.connectToSocket(*this_socket, ip, port);
    if (success) {
        success &= receiver.connectToSocket(*this_socket);
        receiver.setSocket(this_socket);
    }
    if (success) {
        DBG("Connected successfully");
        synchronize_with_X32();
    }

    return success;
}

void OSCConnect::set_Xip(std::string ip)
{
    if (Xip != nullptr)
    {
        delete Xip;
        Xip = nullptr;
    }
    Xip = new juce::IPAddress(ip);
}

void OSCConnect::set_Xport(int port)
{
    PORT_X32 = port;
}