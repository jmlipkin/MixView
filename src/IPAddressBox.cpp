#include "IPAddressBox.h"

juce::String IPAddressBox::filter_text(const juce::String& newInput) {
    juce::String newString = newInput;

    // length validation
    if (newString.length() < MIN_NUM_CHARS_IPV4 || newString.length() > MAX_NUM_CHARS_IPV4)
        return storedString;

    // character validation
    if (!newString.containsOnly(ALLOWED_CHARS))
        return storedString;

    // content validation
    std::vector<int> segments;
    for (int i = 0; i < 4; i++) {
        juce::String seg = newString.upToFirstOccurrenceOf(".",false,true);
        newString = newString.substring(seg.length() + 1);
        int val = seg.getIntValue();
        if (val >= 0 && val <= 255)
            segments.push_back(val);
    }
    // 4 segments parsed correctly
    if(segments.size() == 4) {
        newString = juce::String(segments[0]) + "." + juce::String(segments[1]) + "." + juce::String(segments[2]) + "." + juce::String(segments[3]);

        return newString;
    }

    return storedString;
}

const juce::String& IPAddressBox::get_allowed_chars() {
    return ALLOWED_CHARS;
}