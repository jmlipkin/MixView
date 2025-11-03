#include "X32Connect.h"

#include <juce_core/juce_core.h>

X32Connect::X32Connect() : m_timeout(0)
{
    Xip = new juce::IPAddress("127.0.0.1");
    this_ip = new juce::IPAddress("127.0.0.1");
}

void X32Connect::run()
{
    sender.get_info_X32();
    sender.startThread();
    receiver.startThread();
}

void X32Connect::close(int timeout_milliseconds)
{
    sender.stopThread(timeout_milliseconds);
    receiver.stopThread(timeout_milliseconds);
}

bool X32Connect::connect()
{

    bool success = this_socket.bindToPort(this_port, this_ip->toString());
    if (success)
        success &= sender.connectToSocket(this_socket, Xip->toString(), Xport);
    if (success)
        success &= receiver.connectToSocket(this_socket);
    if (success)
        DBG("Connected successfully");

    return success;
}

bool X32Connect::connect(juce::String ip, int port)
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

void X32Connect::set_Xip(std::string ip)
{
    if (Xip != nullptr)
    {
        free(Xip);
    }
    Xip = new juce::IPAddress(ip);
}

void X32Connect::set_Xport(int port)
{
    Xport = port;
}