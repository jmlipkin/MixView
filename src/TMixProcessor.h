#pragma once

class TMixProcessor {
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

   public:
    juce::ChangeBroadcaster tmix_broadcaster;

    // std::atomic<double> time_since_last_thump_ms;
    std::atomic<std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>> last_thump_time;

   private:
    juce::String cue_number;
    juce::String cue_name;
    Str_Type msg_type;


   public:
    TMixProcessor() {
        last_thump_time = std::chrono::high_resolution_clock::now();
    }

    void set_cue_number(juce::String num) { cue_number = num; }
    juce::String get_cue_number() { return cue_number; }

    void set_cue_name(juce::String name) { cue_name = name; }
    juce::String get_cue_name() { return cue_name; }

    bool is_type_tmix(juce::OSCMessage& message) {
        juce::String address = message.getAddressPattern().toString();

        for (size_t i = 0; i < valid_aps.size(); i++) {
            if (address.startsWith(valid_aps[i])) {
                msg_type = static_cast<Str_Type>(i);
                address = address.substring(valid_aps[i].length());
                return true;
            }
        }
        return false;
    }

    void process_message(juce::OSCMessage& message) {
        switch (msg_type) {
            case CUE_FIRED: {
                cue_number = message[0].getString();
                cue_name = message[1].getString();
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
};