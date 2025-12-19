#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "AssignmentsPopout.h"

class ScribbleStrip : public juce::Component, public juce::ChangeListener {
   public:
    ScribbleStrip(ChannelStrip* ch, std::shared_ptr<MessageProcessor> processor);
    ~ScribbleStrip() override;

    // ChangeListener Implementation
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Component overrides
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;

   private:
    std::unique_ptr<AssignmentsPopout> m_popout;

    // Data references
    ChannelStrip* channel = nullptr;
    std::shared_ptr<MessageProcessor> mp;
    AssignmentContent m_content;
    
    // GUI
    juce::Label channel_id;
    juce::Label channel_name;
    ChannelStrip::COLORS channel_color;

    const std::vector<juce::Colour> SCRIBBLE_BKG = {
        {juce::Colour(0xFF001923)},  // off
        {juce::Colour(0x00000000)},  // red
        {juce::Colour(0xFF055728)},  // green
        {juce::Colour(0xFF767019)},  // yellow
        {juce::Colour(0xFF002538)},  // blues
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
};