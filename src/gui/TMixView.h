#pragma once

#include "../TMixProcessor.h"

class TMixView : public juce::Component, public juce::ChangeListener {
   public:
    TMixView(TMixProcessor* tmix) : tmix_data(tmix) {
        tmix_data->tmix_broadcaster.addChangeListener(this);
        cue_number.setText("---", juce::dontSendNotification);
        cue_number.setFont(juce::FontOptions().withHeight(20));
        cue_number.setJustificationType(juce::Justification::centred);
        cue_name.setText("Run cue change to populate", juce::dontSendNotification);
        cue_name.setFont(juce::FontOptions().withHeight(20));
        cue_name.setJustificationType(juce::Justification::centredLeft);
        cue_name.setMinimumHorizontalScale(1.0f);

        addAndMakeVisible(cue_name);
        addAndMakeVisible(cue_number);
    }

    void paint(juce::Graphics& g) override {
        int corner_radius = 12;
        int bubble_spacing = 6;
        g.setColour(juce::Colour(0xFF002436));
        g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), corner_radius);

        g.setColour(juce::Colour(0xFF02141A));
        g.fillRoundedRectangle(bubble_spacing, bubble_spacing, getWidth() / 5, getHeight() - 2 * bubble_spacing, 9);
        g.fillRoundedRectangle(2 * bubble_spacing + getWidth() / 5, bubble_spacing, getWidth() - (getWidth() / 5 + 3 * bubble_spacing), getHeight() - 2 * bubble_spacing, 9);
    }

    void resized() override {
        cue_number.setBounds(6, 6, getWidth() / 5, getHeight() - 12);
        cue_name.setBounds(2 * 6 + getWidth() / 5 + 6, 6, getWidth() - (getWidth() / 5 + 3 * 6), getHeight() - 2 * 6);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == &tmix_data->tmix_broadcaster) {
            cue_number.setText(tmix_data->get_cue_number(), juce::dontSendNotification);
            cue_name.setText(tmix_data->get_cue_name(), juce::dontSendNotification);
        }
        repaint();
    }

   private:
    TMixProcessor* tmix_data;
    juce::Label cue_number;
    juce::Label cue_name;
};