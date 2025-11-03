#pragma once

#include "ChannelStrip.h"
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

class MessageProcessor {
    ChannelStrip *channels[32];
    ChannelStrip *dcas[8];
};