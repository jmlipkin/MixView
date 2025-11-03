#include "X32Connect.h"

#include <juce_core/juce_core.h>

void X32Connect::run() {
    while (!threadShouldExit())
    {
        sender.resub_to_X32();
        juce::Thread::sleep(m_timeout);
    }

    DBG("Thread ending...");
}

bool X32Connect::connect(){
    return sender.connect(Xip->toString(), Xport);
}

bool X32Connect::connect(juce::String ip, int port)
{
    return sender.connect(ip, port);
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