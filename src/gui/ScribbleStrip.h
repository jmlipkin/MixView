#pragma once

#include "../ChannelStrip.h"
#include <juce_gui_basics/juce_gui_basics.h>

class ScribbleStrip : public juce::Component, public juce::ChangeListener {
    private:
     juce::Colour C_OFF = juce::Colour(0xFF001923);
    //  juce::Colour C_RD;
     juce::Colour C_GN = juce::Colour(0xFF006D36);
     juce::Colour C_YE;
     juce::Colour C_BL;
     juce::Colour C_MG;
     juce::Colour C_CY;
     juce::Colour C_WH = juce::Colour(0xFF002538);
    //  juce::Colour C_GN;
     //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;

     const std::vector<juce::Colour> SCRIBBLE_BKG = {
            {juce::Colour(0xFF001923)}, // off
            {juce::Colour(0x00000000)}, // red
            {juce::Colour(0xFF006D36)}, // green
            {juce::Colour(0xFF767019)}, // yellow
            {juce::Colour(0x00000000)}, // blue
            {juce::Colour(0x00000000)}, // magenta
            {juce::Colour(0x00000000)}, // cyan
            {juce::Colour(0xFF002538)}, // white

     };
     const std::vector<juce::Colour> SCRIBBLE_EDGE = {
            {juce::Colour(0xFF002436)}, // off
            {juce::Colour(0x00000000)}, // red
            {juce::Colour(0xFF6BC974)}, // green
            {juce::Colour(0xFFD1D654)}, // yellow
            {juce::Colour(0x00000000)}, // blue
            {juce::Colour(0x00000000)}, // magenta
            {juce::Colour(0x00000000)}, // cyan
            {juce::Colour(0xFFFFFFFF)}, // white

     };

     ChannelStrip* channel = nullptr;
     juce::Label channel_id;
     juce::Label channel_name;
     ChannelStrip::COLORS channel_color;

    public:
     ScribbleStrip(ChannelStrip* ch);
     ~ScribbleStrip() override;
     void paint(juce::Graphics& g) override;
     void changeListenerCallback(juce::ChangeBroadcaster* source) override;
};