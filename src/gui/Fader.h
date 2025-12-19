#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"

class FaderBkg : public juce::Component {
   public:
    void update_cap_width(int width) { cap_width = width; }
    void paint(juce::Graphics& g) override;

   private:
    int cap_width;
    const juce::Colour C_FADER_BKG = juce::Colour(0xFF002538);
};

class FaderCap : public juce::Component {
   public:
    void paint(juce::Graphics& g) override;

   private:
    juce::Colour C_CAP_BKG = juce::Colour(0xFF868E90);
    juce::Colour C_CAP_GRIP = juce::Colour(0xFFB3B7B9);
    juce::Colour C_CAP_CENTER = juce::Colour(0xFF000000);
};

class Fader : public juce::Component, public juce::ChangeListener {
   public:
    Fader(ChannelStrip* ch);
    ~Fader() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void paint(juce::Graphics& g) override;
    void resized() override;

   private:
    ChannelStrip* channel;
    FaderBkg bkg;
    FaderCap cap;

    juce::Label fader_label;
    float fader_value;

    // drawing parameters
    int cap_height = 64;
    int cap_width = 20;
    int cap_x_pos;
    int cap_y_pos;
    int label_space = 20;
    int range;
};