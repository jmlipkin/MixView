#include "TMixProcessor.h"

TMixProcessor::TMixProcessor() {
    last_thump_time = std::chrono::high_resolution_clock::now();
}

void TMixProcessor::process_message(juce::OSCMessage& message) {
    switch (m_msg_type) {
        case CUE_FIRED: {
            m_cue_number = message[0].getString();
            m_cue_name = message[1].getString();
            tmix_broadcaster.sendChangeMessage();
            return;
        }
        case SUBSCRIBE_OK:
            return;
        case SUBSCRIBE_FAIL:
            return;
        case GANG_LR:
            return;
        case RECORD_OFFSETS:
            return;
        case THEATREMIX:
            return;
        case THUMP:
            last_thump_time.store(std::chrono::high_resolution_clock::now());
            return;
        default:
            return;
    }
}

bool TMixProcessor::is_type_tmix(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();

    for (size_t i = 0; i < valid_aps.size(); i++) {
        if (address.startsWith(valid_aps[i])) {
            m_msg_type = static_cast<Str_Type>(i);
            address = address.substring(valid_aps[i].length());
            return true;
        }
    }
    return false;
}