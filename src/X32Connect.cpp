#include "X32Connect.h"

#include <juce_core/juce_core.h>

void X32Connect::run() {
    while (!threadShouldExit())
    {
        DBG("Xremote!");
        juce::Thread::sleep(m_timeout);
    }

    DBG("Thread ending...");
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