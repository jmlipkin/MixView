#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Macros.h"

class IPAddressBox : public juce::Label {
   public:
    IPAddressBox(juce::String device_name, bool display_name);

    void update_ip();

    void set_isChanged(bool new_state) { m_isChanged = new_state; }
    bool isChanged() { return m_isChanged; }

    juce::IPAddress getIPAddress() { return m_addr; }

    void setDeviceName(juce::String name) { m_device_name.setText(name, juce::dontSendNotification); }
    juce::String getDeviceName() { return m_device_name.getText();  }

   private:
    // Sets a new value programmatically and stores it as valid
    void setIPAddress(const juce::String& newAddress);
    juce::String filter_text(const juce::String& str);

    const juce::String& get_allowed_chars();

   private:
    juce::IPAddress m_addr;
    juce::Label m_device_name;

    juce::String m_storedString;

    bool m_name_should_be_displayed;
    bool m_isChanged = false;

    static const int MIN_NUM_CHARS_IPV4{7};
    static const int MAX_NUM_CHARS_IPV4{15};

    static inline const juce::String ALLOWED_CHARS{"1234567890."};
};