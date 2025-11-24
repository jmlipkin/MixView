#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Fader.h"
#include "ScribbleStrip.h"
#include "ChannelState.h"

class StripView : public juce::Component {
   private:
    ChannelStrip* channel_data;

    std::unique_ptr<ScribbleStrip> scribble;
    std::unique_ptr<ChannelState> state;
    std::unique_ptr<Fader> fader;

   public:
    StripView(ChannelStrip* channel) : channel_data(channel) {
        fader = std::make_unique<Fader>(channel_data);
        state = std::make_unique<ChannelState>(channel_data);
        scribble = std::make_unique<ScribbleStrip>(channel_data);

        addAndMakeVisible(scribble.get());
        addAndMakeVisible(state.get());
        addAndMakeVisible(fader.get());

        float scribble_height = 0.12f * getHeight();
        float fader_height = 0.8f * getHeight();
        float state_height = 0.05f * getHeight();
        float spacer = 0.01f * getHeight();

        scribble->setBounds(0, 0, getWidth(), (int)scribble_height);
        fader->setBounds(0, int(scribble_height + 2 * spacer), getWidth(), (int)fader_height);
        state->setBounds(0, int(scribble_height + fader_height + 3 * spacer), getWidth(), (int)state_height);
    }

    // TODO: Do I need this??
    // void paint(juce::Graphics& g) override {}

    void resized() override {
        float scribble_height = 0.12f * getHeight();
        float fader_height = 0.8f * getHeight();
        float state_height = 0.05f * getHeight();
        float spacer = 0.01f * getHeight();

        scribble->setBounds(0, 0, getWidth(), (int)scribble_height);
        fader->setBounds(0, (int)(scribble_height + 2 * spacer), getWidth(), (int)fader_height);
        state->setBounds(0, (int)(scribble_height + fader_height + 3 * spacer), getWidth(), (int)state_height);
    }
};