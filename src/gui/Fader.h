#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"


class FaderBkg : public juce::Component {
    private:
    public:
     const juce::Colour C_FADER_BKG = juce::Colour(0xFF002538);

     void paint(juce::Graphics& g) override {
         g.fillAll(C_FADER_BKG);
     }
};


class FaderCap : public juce::Component {
   public:

    void paint(juce::Graphics& g) override {
    }
};

class Fader : public juce::Component, public juce::ChangeListener {
    ChannelStrip* channel;
    FaderBkg bkg;
    FaderCap cap;

    juce::Label fader_label;
    float fader_value;

   public:
    Fader(ChannelStrip* ch) : channel(ch) {
        channel->fader_broadcaster.addChangeListener(this);
        fader_value = channel->get_fader_value();

        
        addAndMakeVisible(bkg);
        bkg.setBounds(0, 30, getWidth(), getHeight() - 30);
    }
    ~Fader() {
        channel->fader_broadcaster.removeChangeListener(this);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colour(0xFF002538).brighter());

        bkg.paint(g);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == &channel->fader_broadcaster) {
            fader_value = channel->get_fader_value();
        }
        repaint();
    }
};