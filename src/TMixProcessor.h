#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

typedef std::atomic<std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>> TMixTimePoint;

class TMixProcessor {
   public:
    juce::ChangeBroadcaster tmix_broadcaster;

    TMixTimePoint last_thump_time;

   public:
    // Sets last_thump_time to time at construction call.
    TMixProcessor();

    void set_cue_number(juce::String num) { m_cue_number = num; }
    juce::String get_cue_number() { return m_cue_number; }

    void set_cue_name(juce::String name) { m_cue_name = name; }
    juce::String get_cue_name() { return m_cue_name; }

    // Takes a reference to a juce::OSCMessage and responds according to type
    // of message as defined by TMixProcessor::Str_Type. Currently, only
    // CUE_FIRED and THUMP messages do anything.
    void process_message(juce::OSCMessage& message);

    // Returns TRUE if message address pattern matches one of the valid
    // ThreatreMix address patterns, as defined by TMixProcessor::valid_aps
    bool is_type_tmix(juce::OSCMessage& message);

   private:
    enum Str_Type {
        SUBSCRIBE_OK,
        SUBSCRIBE_FAIL,
        CUE_FIRED,
        GANG_LR,
        RECORD_OFFSETS,
        THEATREMIX,
        THUMP
    };

    std::vector<juce::String> valid_aps = {
        "/subscribeok",
        "/subscribefail",
        "/cuefired",
        "/ganglr",
        "/recordoffsets",
        "/theatremix",
        "/thump"};

   private:
    juce::String m_cue_number;
    juce::String m_cue_name;
    Str_Type m_msg_type;
};