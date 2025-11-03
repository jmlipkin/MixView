#pragma once

#include <juce_graphics/juce_graphics.h>

class Fader : juce::Component {
    private:
        float value;
        juce::Label printout;

    public:
        void set_value(float val) { value = val; }
        
        void paint() override {}
};