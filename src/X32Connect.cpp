#include "X32Connect.h"

#include <juce_core/juce_core.h>

X32Connect::X32Connect() : juce::Thread("X32Connect"), m_timeout(0) {
    Xip = new juce::IPAddress("127.0.0.1");
    this_ip = new juce::IPAddress("127.0.0.1");
}

void X32Connect::run() {
    while (!threadShouldExit())
    {
        sender.resub_to_X32();
        juce::Thread::sleep(m_timeout);
    }

    DBG("Thread ending...");
}

bool X32Connect::connect(){

    this_socket.bindToPort(this_port, this_ip->toString());

    bool rec_success = false;
    bool send_success = sender.connectToSocket(this_socket, Xip->toString(), Xport);
    if(send_success) {
        DBG("Send connection established.");
        rec_success = receiver.connect_and_start(this_socket);
    }
    if(rec_success) {
        DBG("Receive connection established.");
    }
    return (send_success && rec_success);
}

bool X32Connect::connect(juce::String ip, int port) {
    this_socket.bindToPort(this_port, this_ip->toString());
    
    bool rec_success = false;
    bool send_success = sender.connectToSocket(this_socket, ip, port);
    if(send_success) {
        DBG("Send connection established.");
        rec_success = receiver.connect_and_start(this_socket);
    }
    if(rec_success) {
        DBG("Receive connection established.");
    }
    return (send_success && rec_success);
}

void X32Connect::set_Xip(std::string ip) {
    if(Xip != nullptr){
        free(Xip);
    }
    Xip = new juce::IPAddress(ip);
}

void X32Connect::set_Xport(int port) {
    Xport = port;
}