#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../IPAddressBox.h"
#include "../OSCConnect.h"
#include "DeviceState.h"

class InfoBar : public juce::Component {
   public:
    InfoBar(MessageProcessor& processor);
    ~InfoBar() override;

    // Connect button callback
    void startButtonClicked();

    // Component overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

   private:
    std::unique_ptr<OSCConnect> connector;

    // Connect button
    juce::TextButton starter;

    // Data references
    MessageProcessor& mp;

    // Device labels
    juce::Label label_host;
    juce::Label label_console;
    juce::Label label_tmix;

    // Connection status indicators
    DeviceState state_console;
    DeviceState state_tmix;

    // IP Address editors
    IPAddressBox ip_host{"This Device", false};
    IPAddressBox ip_console{"Behringer X32", true};
    IPAddressBox ip_tmix{"TheatreMix", true};
};
