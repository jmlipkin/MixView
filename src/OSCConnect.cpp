#include "OSCConnect.h"

#include <juce_core/juce_core.h>

#define IP_X32 "10.5.136.59"

OSCConnect::OSCConnect() : m_timeout(0)
{
    Xip = new juce::IPAddress(IP_X32);
    this_ip = new juce::IPAddress(IP_X32);
}

void OSCConnect::run()
{
    sender.get_info_X32();
    sender.startThread();
    receiver.open();
}

void OSCConnect::close(int timeout_milliseconds)
{
    sender.stopThread(timeout_milliseconds);
    receiver.close();
}

bool OSCConnect::connect()
{

    bool success = this_socket.bindToPort(this_port, this_ip->toString());
    if (success)
        success &= sender.connectToSocket(this_socket, Xip->toString(), PORT_X32);
    if (success) {
        success &= receiver.connectToSocket(this_socket);
        receiver.setSocket(&this_socket);
    }
    if (success)
        DBG("Connected successfully");

    return success;
}

bool OSCConnect::connect(juce::String ip, int port)
{

    bool success = this_socket.bindToPort(this_port, this_ip->toString());
    if (success)
        success &= sender.connectToSocket(this_socket, ip, port);
    if (success)
        success &= receiver.connectToSocket(this_socket);
    if (success)
        DBG("Connected successfully");

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