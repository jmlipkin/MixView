#include "Receiver.h"

Receiver::Receiver(MessageProcessor& processor) : mp(processor) {}

Receiver::~Receiver() {
    close();
}

void Receiver::open() { addListener(this); }

void Receiver::close() {
    removeListener(this);
    disconnect();
}

void Receiver::oscMessageReceived(const juce::OSCMessage &message) {
    mp.add_to_buffer(message);
}