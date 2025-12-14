#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include <deque>

#include "ChannelStrip.h"
#include "TMixProcessor.h"

typedef std::atomic<std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::steady_clock::duration>> MPTimePoint;

class MessageProcessor : public juce::Thread {
   public:
    MPTimePoint last_thump_time;

   public:
    // Allocates all ChannelStrip members and TMixProcessor.
    // Also sets Address Pattern and ID for ChannelStrips and sets
    // last_thump_time to current time.
    MessageProcessor();

    // Destroys all ChannelStrip members and TMixProcessor
    ~MessageProcessor() override;

    // Adds message to back of messages buffer and wakes thread alerting a 
    // message is ready
    void add_to_buffer(juce::OSCMessage message);

    // Starts thread. Processes all messages in the buffer. Sleeps when buffer
    // is empty. Woken when add_to_buffer is called.
    void run() override;

    // Signals thread should stop with timeout of 1 sec.
    void stop();

    // Getter functions -- single element only

    InputChannelStrip* get_in_ch(int idx) { return &m_channels[idx]; }
    InputChannelStrip* get_auxin(int idx) { return &m_auxins[idx]; }
    InputChannelStrip* get_bus(int idx) { return &m_buses[idx]; }
    ChannelStrip* get_mtx(int idx) { return &m_matrices[idx]; }
    ChannelStrip* get_main_st() { return m_main_st; }
    DCAChannelStrip* get_dca(int idx) { return &m_dcas[idx]; }
    TMixProcessor* get_tmix() { return m_tmix; }

   private:
    // Main dispatching logic for message
    void process_message(juce::OSCMessage& message);

    // Catch function for messages that are not dispatched
    void print_unprocessed_string(juce::OSCMessage& message);

    // Helper function for process_message: identifies which vector to search
    ChannelStrip::CH_TYPE find_channel_type(juce::OSCMessage& message);
    // Helper function for process_message: identifies which element of vector
    // the message refers to
    size_t find_channel_idx(juce::OSCMessage& message, ChannelStrip::CH_TYPE type);

    // Helper function for print_unprocessed_string
    juce::String arg_to_str(const juce::OSCArgument& arg);

   private:
    InputChannelStrip* m_channels;
    InputChannelStrip* m_auxins;
    InputChannelStrip* m_buses;
    ChannelStrip* m_matrices;
    ChannelStrip* m_main_st;
    DCAChannelStrip* m_dcas;
    TMixProcessor* m_tmix;

    std::deque<juce::OSCMessage> m_msg_buffer;

    const size_t NUM_IN_CH = 32;
    const size_t NUM_AUXINS = 8;
    const size_t NUM_BUSES = 16;
    const size_t NUM_MATRICES = 6;
    const size_t NUM_DCAS = 8;

    std::mutex mutex;
    juce::WaitableEvent messageReady;
};