#include "ChannelStrip.h"


void ChannelStrip::set_num_and_ap(size_t idx) {
    if(idx + 1 < 10)
        number.ap = "/0" + juce::String(idx + 1);
    else
        number.ap = "/" + juce::String(idx + 1);
    number.index = idx;
}

const std::vector<juce::String> ChannelStrip::channel_type = {"/ch", "/auxin", "/bus", "/mtx", "/dca"};