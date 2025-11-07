#pragma once

#include "../ChannelStrip.h"
#include <juce_gui_basics/juce_gui_basics.h>

class State : public juce::Component, public juce::ChangeListener {
    private:
     ChannelStrip* channel = nullptr;
     bool mute_color;
     juce::Colour C_STATE_ON = juce::Colour(0xFF474040);
     juce::Colour C_STATE_OFF = juce::Colour(0xFF5B0001);

    public:
     State(ChannelStrip* ch) : channel(ch) {
         channel->state_broadcaster.addChangeListener(this);

         mute_color = channel->get_state();
     }

     ~State() override {
         channel->state_broadcaster.removeChangeListener(this);
     }

     void paint (juce::Graphics &g) override {
         int corner_radius = 5;
         g.setColour(mute_color ? C_STATE_ON : C_STATE_OFF);
         g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), corner_radius);
     }

     void changeListenerCallback(juce::ChangeBroadcaster* source) override {
         if (source == &channel->state_broadcaster) {
             mute_color = channel->get_state();
         }
         repaint();
     }
};
