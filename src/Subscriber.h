#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_osc/juce_osc.h>

#include "Macros.h"
#include "MessageProcessor.h"
#include "TMixProcessor.h"

class Subscriber : public juce::OSCSender, public juce::Thread {
   public:
    juce::ChangeBroadcaster state_broadcaster;

   public:
    Subscriber();
    ~Subscriber() override;

    // Checks if last thump time of relevant device is within timeout value.
    // Returns that value. If connection status is different from previous,
    // sends a state_broadcaster change message.
    void check_if_connected();

    // Must be called in container's constructor
    void set_port(int port) { m_port = port; }
    void set_tmix(TMixProcessor* tmix_loc) { tmix = tmix_loc; }
    void set_mp(MessageProcessor* mp_loc) { mp = mp_loc; }

    void set_timeout(int timeout) { m_timeout = timeout; }
    int get_timeout() const { return m_timeout; }

   private:
    // On initial call, sends appropriate get_info function depending on stored
    // port. Run loop continuously sends appropriate resub function at interval
    // determined by timeout.
    void run() override;

    // Sends /info
    void get_info_X32();
    // Sends /theatremix
    void get_info_tmix();

    // Sends /xremote request and polls for info
    void resub_to_x32();
    // Sends /subscribe request and polls for thump
    void resub_to_tmix();

   private:
    static const int PORT_X32{10023};
    static const int PORT_TMIX{32000};

    int m_port;

    int m_timeout{200};
    bool m_connected{false};

    TMixProcessor* tmix{nullptr};
    MessageProcessor* mp{nullptr};
};