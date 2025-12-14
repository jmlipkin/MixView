#include "IPAddressBox.h"

IPAddressBox::IPAddressBox(juce::String device_name, bool display_name) {
    m_addr = juce::IPAddress();
    setEditable(true);
    onTextChange = nullptr;  // We don't validate on every keystroke

    juce::Label::setColour(juce::Label::backgroundColourId, juce::Colour(0xFF02141A));
    juce::Label::setColour(juce::Label::textColourId, juce::Colour(0xFFFFFFFF));
    setText(m_addr.toString(), juce::dontSendNotification);
    setJustificationType(juce::Justification::centred);

    m_device_name.setText(device_name, juce::dontSendNotification);
    m_name_should_be_displayed = display_name;

    if(m_name_should_be_displayed)
    {
        addAndMakeVisible(m_device_name);
    }
}

void IPAddressBox::update_ip() {
    auto text = filter_text(getText(true));
    if (m_storedString != text) {
        setIPAddress(text);
        m_storedString = text;
        m_isChanged = true;
    }
    setText(text, juce::dontSendNotification);
}

void IPAddressBox::setIPAddress(const juce::String& newAddress) {
    m_storedString = newAddress;
    m_addr = juce::IPAddress(newAddress);
}

juce::String IPAddressBox::filter_text(const juce::String& newInput) {
    juce::String newString = newInput;

    // length validation
    if (newString.length() < MIN_NUM_CHARS_IPV4 || newString.length() > MAX_NUM_CHARS_IPV4)
        return m_storedString;

    // character validation
    if (!newString.containsOnly(ALLOWED_CHARS))
        return m_storedString;

    // content validation
    std::vector<int> segments;
    for (int i = 0; i < 4; i++) {
        juce::String seg = newString.upToFirstOccurrenceOf(".", false, true);
        newString = newString.substring(seg.length() + 1);
        int val = seg.getIntValue();
        if (val >= 0 && val <= 255)
            segments.push_back(val);
    }
    // 4 segments parsed correctly
    if (segments.size() == 4) {
        newString = juce::String(segments[0]) + "." + juce::String(segments[1]) + "." + juce::String(segments[2]) + "." + juce::String(segments[3]);

        return newString;
    }

    return m_storedString;
}

const juce::String& IPAddressBox::get_allowed_chars() {
    return ALLOWED_CHARS;
}