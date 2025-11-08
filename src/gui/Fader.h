#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"

class FaderBkg : public juce::Component {
   private:
    int cap_width;

   public:
    void update_cap_width(int width) { cap_width = width; }
    const juce::Colour C_FADER_BKG = juce::Colour(0xFF002538);

    void paint(juce::Graphics& g) override {
        g.setColour(C_FADER_BKG);
        g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4);
        // g.setColour(juce::Colour(0xFF7D7C77));
        // g.fillRoundedRectangle(0, 0, getWidth(), 2, 2);
        // g.fillRoundedRectangle(0, getHeight() - 2, getWidth(), 2, 2);

        int cap_x_pos = (getWidth() - cap_width) / 2;

        int center_width = 6;
        int bkg_x_pos = cap_x_pos + (cap_width + center_width) / 2;

        g.setColour(juce::Colour(0xFF02141A));
        g.fillRoundedRectangle(bkg_x_pos, 0, center_width, getHeight(), 4);
    }
};

class FaderCap : public juce::Component {
   private:
    juce::Colour C_CAP_BKG = juce::Colour(0xFF868E90);
    juce::Colour C_CAP_GRIP = juce::Colour(0xFFB3B7B9);
    juce::Colour C_CAP_CENTER = juce::Colour(0xFF000000);

   public:
    void paint(juce::Graphics& g) override {
        g.setColour(C_CAP_BKG);
        g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 2);

        g.setColour(C_CAP_GRIP);

        int dist = getHeight() / 16;
        int line_height = 1;
        int line_width = int(float(getWidth()) * 0.8);
        int h_offset = (getWidth() - line_width) / 2;
        int line_corner_radius = 1;

        for (int i = 0; i < 16; i++) {
            if (i == 0 || i == 6 || i == 7 || i == 9 || i == 10)
                continue;
            if (i == 8) {
                g.setColour(C_CAP_CENTER);
                g.fillRoundedRectangle(0, dist * i, getWidth(), line_height + 1, line_corner_radius);
                g.setColour(C_CAP_GRIP);
                continue;
            }
            g.fillRoundedRectangle(h_offset, dist * i, line_width, line_height + 0.5, line_corner_radius);
        }
    }
};

class Fader : public juce::Component, public juce::ChangeListener {
    ChannelStrip* channel;
    FaderBkg bkg;
    FaderCap cap;

    juce::Label fader_label;
    float fader_value;

    int cap_height = 64;
    int cap_width = 20;
    int cap_x_pos;
    int cap_y_pos;
    int label_space = 20;
    int range;

   public:
    Fader(ChannelStrip* ch) : channel(ch) {
        channel->fader_broadcaster.addChangeListener(this);
        fader_value = channel->get_fader_value();

        range = getHeight() - label_space - cap_height;

        fader_label.setText(juce::String(channel->get_fader_value()) + " dB", juce::dontSendNotification);
        fader_label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(fader_label);

        bkg.update_cap_width(cap_width);

        addAndMakeVisible(bkg);
        addAndMakeVisible(cap);
    }
    ~Fader() override {
        channel->fader_broadcaster.removeChangeListener(this);
    }

    void paint(juce::Graphics& g) override {
        int corner_radius = 4;
        g.setColour(juce::Colour(0xFF002436));
        g.fillRoundedRectangle(0, 0, getWidth(), label_space, corner_radius);
    }

    void resized() override {
        range = getHeight() - label_space - cap_height;
        cap_x_pos = (getWidth() - cap_width) / 2;
        cap_y_pos = static_cast<int>((float)label_space + (float)range * (1.f - fader_value));

        fader_label.setBounds(0, 0, getWidth(), label_space);
        bkg.setBounds(0, label_space + cap_height / 2, getWidth(), getHeight() - label_space - cap_height);
        cap.setBounds(cap_x_pos, cap_y_pos, getWidth() / 3, cap_height);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == &channel->fader_broadcaster) {
            fader_value = channel->get_fader_value();
            cap_y_pos = static_cast<int>((float)label_space + (float)range * (1.f - fader_value));
            cap.setBounds(cap_x_pos, cap_y_pos, getWidth() / 3, cap_height);
            cap.repaint();
            if (channel->get_fader_label().compare("-90") == 0) {
                fader_label.setText("-inf dB", juce::dontSendNotification);
            } else {
                fader_label.setText(channel->get_fader_label() + " dB", juce::dontSendNotification);
            }

            fader_label.repaint();
        }
        repaint();
    }
};