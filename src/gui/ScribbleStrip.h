#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"

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
    //      juce::Colour C_WHi = juce::Colour();

    const std::vector<juce::Colour> SCRIBBLE_BKG = {
        {juce::Colour(0xFF001923)},  // off
        {juce::Colour(0x00000000)},  // red
        {juce::Colour(0xFF055728)},  // green
        {juce::Colour(0xFF767019)},  // yellow
        {juce::Colour(0xFF002538)},  // blue
        {juce::Colour(0xFF4A2A67)},  // magenta
        {juce::Colour(0xFF00707C)},  // cyan
        {juce::Colour(0xFF002538)},  // white
        {juce::Colour(0x00000000)},  // offi
        {juce::Colour(0x00000000)},  // redi
        {juce::Colour(0xFF2E8C52)},  // greeni
        {juce::Colour(0xFF767019)},  // yellowi
        {juce::Colour(0x00000000)},  // bluei
        {juce::Colour(0xFF693398)},  // magentai
        {juce::Colour(0xFF59A9B4)},  // cyani
        {juce::Colour(0xFF324855)}   // whitei

    };
    const std::vector<juce::Colour> SCRIBBLE_EDGE = {
        {juce::Colour(0xFF002436)},  // off
        {juce::Colour(0x00000000)},  // red
        {juce::Colour(0xFF89FF96)},  // green
        {juce::Colour(0xFFD1D654)},  // yellow
        {juce::Colour(0xFF0083B8)},  // blue
        {juce::Colour(0xFFD92BF9)},  // magenta
        {juce::Colour(0xFF00F2F5)},  // cyan
        {juce::Colour(0xFFFFFFFF)},  // white
        {juce::Colour(0x00000000)},  // offi
        {juce::Colour(0x00000000)},  // redi
        {juce::Colour(0xFF89FF96)},  // greeni
        {juce::Colour(0xFFD1D654)},  // yellowi
        {juce::Colour(0x00000000)},  // bluei
        {juce::Colour(0xFFD92BF9)},  // magentai
        {juce::Colour(0x00000000)},  // cyani
        {juce::Colour(0xFFBBBBBB)}   // whitei
    };

    ChannelStrip* channel = nullptr;
    juce::Label channel_id;
    juce::Label channel_name;
    ChannelStrip::COLORS channel_color;

   public:
    ScribbleStrip(ChannelStrip* ch);
    ~ScribbleStrip() override;
    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
};