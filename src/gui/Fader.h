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
        g.fillAll(juce::Colour(0xFFFFFFFF));
    }
};

class Fader : public juce::Component, public juce::ChangeListener {
    ChannelStrip* channel;
    FaderBkg bkg;
    FaderCap cap;

    juce::Label fader_label;
    float fader_value;

    int cap_height = 3;
    int label_space = 30;
    int range;

   public:
    Fader(ChannelStrip* ch) : channel(ch) {
        channel->fader_broadcaster.addChangeListener(this);
        fader_value = channel->get_fader_value();

        range = getWidth() - label_space;

        fader_label.setText(juce::String(channel->get_fader_value()) + " dB", juce::dontSendNotification);
        fader_label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(fader_label);

        addAndMakeVisible(bkg);
        addAndMakeVisible(cap);
    }
    ~Fader() override {
        channel->fader_broadcaster.removeChangeListener(this);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colour(0xFF002538).brighter());
    }
    void resized() override {
        range = getHeight() - label_space - cap_height;
        int cap_pos = label_space + range * (1 - fader_value) - ( cap_height / 2 );
        fader_label.setBounds(0, 0, getWidth(), label_space);
        bkg.setBounds(0, label_space, getWidth(), getHeight() - label_space);
        bkg.setBounds(0, label_space, getWidth(), getHeight() - label_space);
        cap.setBounds(0, cap_pos, getWidth(), cap_height);
    }
    
    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == &channel->fader_broadcaster) {
            fader_value = channel->get_fader_value();
            int cap_pos = label_space + range * (1 - fader_value) - cap_height / 2;
            cap.setBounds(0, cap_pos, getWidth(), cap_height);
            cap.repaint();
            fader_label.setText(channel->get_fader_label() + " dB", juce::dontSendNotification);
            fader_label.repaint();
        }
        repaint();
    }
};