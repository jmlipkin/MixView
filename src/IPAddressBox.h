#pragma once

#include "Macros.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class IPAddressBox : public juce::Label {
   private:
    std::unique_ptr<juce::IPAddress> addr;
    juce::String storedString;

   public:
    IPAddressBox() {
        addr = std::make_unique<juce::IPAddress>();
        setEditable(true);
        onTextChange = nullptr;  // We don't validate on every keystroke

        setColour(juce::Label::backgroundColourId, juce::Colour(0xFF0000DD));
        setColour(juce::Label::textColourId, juce::Colour(0xFFFFFFFF));
        setText(addr->toString(), juce::dontSendNotification);
        setJustificationType(juce::Justification::centred);

    }

    // const std::unique_ptr<juce::IPAddress>& getIPAddress() const {
    //     return addr;
    // }

    void update_ip() {
        auto text = filter_text(getText(true));
        if (storedString != text) {
            setIPAddress(text);
            storedString = text;
            m_isChanged = true;
            setText(storedString, juce::dontSendNotification);
        }
    }

    juce::IPAddress& getIPAddress() { return *addr; }

    bool isChanged() { return m_isChanged; }
    void set_isChanged(bool new_state) { m_isChanged = new_state; }

   protected:
    // void textEditorReturnKeyPressed(juce::TextEditor& editor) override { update_ip(); }

   private:
    static const int MIN_NUM_CHARS_IPV4{7};
    static const int MAX_NUM_CHARS_IPV4{15};
    static inline const juce::String ALLOWED_CHARS{"1234567890."};
    bool m_isChanged;

    juce::String filter_text(const juce::String& str);
    const juce::String& get_allowed_chars();
    // Sets a new value programmatically and stores it as valid
    void setIPAddress(const juce::String& newAddress) {
        storedString = newAddress;
        setText(newAddress, juce::dontSendNotification);
        addr = std::make_unique<juce::IPAddress>(newAddress);
    }
};