#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include <deque>

#include "ChannelStrip.h"
#include "TMixProcessor.h"

class MessageProcessor : public juce::Thread {
   private:
    const size_t NUM_IN_CH = 32;
    const size_t NUM_AUXINS = 8;
    const size_t NUM_BUSES = 16;
    const size_t NUM_MATRICES = 6;
    const size_t NUM_DCAS = 8;

    InputChannelStrip* channels;
    InputChannelStrip* auxins;
    InputChannelStrip* buses;
    ChannelStrip* matrices;
    ChannelStrip* main_st;
    DCAChannelStrip* dcas;
    TMixProcessor* tmix;

    std::deque<juce::OSCMessage> msg_buffer;

    std::mutex mtx;
    juce::WaitableEvent messageReady;

   public:
    MessageProcessor() : juce::Thread("MessageProcess") {
        channels = new InputChannelStrip[NUM_IN_CH];
        auxins = new InputChannelStrip[NUM_AUXINS];
        buses = new InputChannelStrip[NUM_BUSES];
        matrices = new ChannelStrip[NUM_MATRICES];
        main_st = new ChannelStrip;
        dcas = new DCAChannelStrip[NUM_DCAS];
        tmix = new TMixProcessor;

        for (size_t i = 0; i < NUM_IN_CH; i++) {
            channels[i].set_id("CH " + juce::String(i + 1));
            channels[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_AUXINS; i++) {
            auxins[i].set_id("AUX " + juce::String(i + 1));
            auxins[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_BUSES; i++) {
            buses[i].set_id("BUS " + juce::String(i + 1));
            buses[i].set_num_and_ap(i);
        }
        for (size_t i = 0; i < NUM_MATRICES; i++) {
            matrices[i].set_id("MTX " + juce::String(i + 1));
            matrices[i].set_num_and_ap(i);
        }
        main_st->set_id("LR");
        for (size_t i = 0; i < NUM_DCAS; i++) {
            dcas[i].set_id("DCA " + juce::String(i + 1));
            dcas[i].set_num_and_ap(i);
        }
    }
    ~MessageProcessor() override {
        delete[] channels;
        delete[] auxins;
        delete[] buses;
        delete[] matrices;
        delete[] dcas;
        delete main_st;
        delete tmix;

        channels = nullptr;
        auxins = nullptr;
        buses = nullptr;
        matrices = nullptr;
        dcas = nullptr;
        main_st = nullptr;
        tmix = nullptr;

        stop();
    }

    InputChannelStrip* get_in_ch(int idx) { return &channels[idx]; }
    InputChannelStrip* get_auxin(int idx) { return &auxins[idx]; }
    InputChannelStrip* get_bus(int idx) { return &buses[idx]; }
    ChannelStrip* get_mtx(int idx) { return &matrices[idx]; }
    ChannelStrip* get_main_st() { return main_st; }
    DCAChannelStrip* get_dca(int idx) { return &dcas[idx]; }
    TMixProcessor* get_tmix() { return tmix; }

    juce::String arg_to_str(const juce::OSCArgument& arg) {
        switch (arg.getType()) {
            case 'f':
                return juce::String(arg.getFloat32());
            case 'i':
                return juce::String(arg.getInt32());
            case 's':
                return arg.getString();

            default:
                return juce::String(" + 1TYPE NOT HANDLED");
                break;
        }
    }

    void add_to_buffer(juce::OSCMessage message) {
        std::lock_guard<std::mutex> lock(mtx);
        msg_buffer.push_back(message);

        messageReady.signal();

        if (!isThreadRunning())
            startThread();
    }

    void run() override {
        while (!threadShouldExit()) {
            messageReady.wait(-1);

            while (true) {
                juce::OSCMessage message{"/null_from_mp_thread"};
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (msg_buffer.empty()) {
                        break;
                    }
                    message = std::move(msg_buffer.front());
                    msg_buffer.pop_front();
                }

                process_message(message);
            }
        }
    }

    void stop() {
        signalThreadShouldExit();
        messageReady.signal();  
        stopThread(1000);    
    }

    void process_message(juce::OSCMessage& message) {
        size_t idx = 100;
        switch (find_channel_type(message)) {
            case ChannelStrip::CH_TYPE::IN_CH: {
                idx = find_channel_idx(message, ChannelStrip::CH_TYPE::IN_CH);
                channels[idx].update_parameter(message);

                break;
            }
            case ChannelStrip::CH_TYPE::AUX_IN: {
                idx = find_channel_idx(message, ChannelStrip::CH_TYPE::AUX_IN);
                auxins[idx].update_parameter(message);

                break;
            }
            case ChannelStrip::CH_TYPE::BUS: {
                idx = find_channel_idx(message, ChannelStrip::CH_TYPE::BUS);
                buses[idx].update_parameter(message);

                break;
            }
            case ChannelStrip::CH_TYPE::DCA: {
                idx = find_channel_idx(message, ChannelStrip::CH_TYPE::DCA);
                dcas[idx].update_parameter(message);

                break;
            }
            case ChannelStrip::CH_TYPE::MATRIX: {
                idx = find_channel_idx(message, ChannelStrip::CH_TYPE::MATRIX);
                matrices[idx].update_parameter(message);
                break;
            }
            case ChannelStrip::CH_TYPE::MAIN_ST: {
                main_st->update_parameter(message);
                break;
            }
            case ChannelStrip::CH_TYPE::INVALID: {
                if(tmix->is_type_tmix(message))
                {
                    tmix->process_message(message);
                    break;
                }
                print_unprocessed_string(message);
                break;
            }
            default:
                print_unprocessed_string(message);
                break;
        }
    }

    ChannelStrip::CH_TYPE find_channel_type(juce::OSCMessage& message) {
        juce::String address = message.getAddressPattern().toString();

        for (size_t i = 0; i < ChannelStrip::channel_type.size(); i++) {
            if (address.startsWith(ChannelStrip::channel_type[i])) {
                address = address.substring(ChannelStrip::channel_type[i].length());
                message.setAddressPattern(address);
                return static_cast<ChannelStrip::CH_TYPE>(i);
            }
        }

        return ChannelStrip::INVALID;
    }

    size_t find_channel_idx(juce::OSCMessage& message, ChannelStrip::CH_TYPE type) {
        juce::String address = message.getAddressPattern().toString();

        switch (type) {
            case ChannelStrip::IN_CH:
                for (size_t i = 0; i < NUM_IN_CH; i++) {
                    if (address.startsWith(channels[i].get_num_ap().toString())) {
                        address = address.substring(channels[i].get_num_ap().toString().length());
                        message.setAddressPattern(address);
                        return i;
                    }
                }
                break;
            case ChannelStrip::CH_TYPE::AUX_IN: {
                for (size_t i = 0; i < NUM_AUXINS; i++) {
                    if (address.startsWith(auxins[i].get_num_ap().toString())) {
                        address = address.substring(auxins[i].get_num_ap().toString().length());
                        message.setAddressPattern(address);
                        return i;
                    }
                }
            }
            case ChannelStrip::CH_TYPE::BUS: {
                for (size_t i = 0; i < NUM_BUSES; i++) {
                    if (address.startsWith(buses[i].get_num_ap().toString())) {
                        address = address.substring(buses[i].get_num_ap().toString().length());
                        message.setAddressPattern(address);
                        return i;
                    }
                }
            }
            case ChannelStrip::CH_TYPE::DCA: {
                for (size_t i = 0; i < NUM_DCAS; i++) {
                    if (address.startsWith(dcas[i].get_num_ap().toString())) {
                        address = address.substring(dcas[i].get_num_ap().toString().length());
                        message.setAddressPattern(address);
                        return i;
                    }
                }
            }
            case ChannelStrip::CH_TYPE::MATRIX: {
                for (size_t i = 0; i < NUM_MATRICES; i++) {
                    if (address.startsWith(matrices[i].get_num_ap().toString())) {
                        address = address.substring(matrices[i].get_num_ap().toString().length());
                        message.setAddressPattern(address);
                        return i;
                    }
                }
            }
            case ChannelStrip::CH_TYPE::MAIN_ST: {
                break;
            }
            case ChannelStrip::CH_TYPE::INVALID:
                return 100;
            default:
                return 100;
        }
        return 100;
    }

    void print_unprocessed_string(juce::OSCMessage& message) {
        juce::String address = message.getAddressPattern().toString();
        int num_args = message.size();
        juce::String msg_str = address;

        for (int i = 0; i < num_args; i++) {
            msg_str += " " + arg_to_str(message[i]);
        }

        DBG(msg_str);
    }
};