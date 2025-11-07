#include "ScribbleStrip.h"

#include "../ChannelStrip.h"

ScribbleStrip::ScribbleStrip(ChannelStrip* ch) : channel(ch) {
    channel->scribble_broadcaster.addChangeListener(this);

    channel_id.setText(channel->get_id(), juce::dontSendNotification);
    channel_id.setBounds(20, 5, 80, 20);
    channel_name.setText(channel->get_name(), juce::dontSendNotification);
    channel_name.setBounds(20, 25, 80, 20);

    addAndMakeVisible(channel_id);
    addAndMakeVisible(channel_name);
}

ScribbleStrip::~ScribbleStrip() {
    channel->scribble_broadcaster.removeChangeListener(this);
}

void ScribbleStrip::paint(juce::Graphics& g) {
    int line_width = 2;
    int corner_radius = 10;
    g.setColour(SCRIBBLE_EDGE[channel_color]);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), corner_radius);
    g.setColour(SCRIBBLE_BKG[channel_color]);
    g.fillRoundedRectangle(line_width, line_width, getWidth() - 2 * line_width, getHeight() - 2 * line_width, corner_radius - line_width);
}

void ScribbleStrip::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &channel->scribble_broadcaster) {
        channel_color = channel->get_color_value();
        channel_id.setText(channel->get_id(), juce::dontSendNotification);
        channel_name.setText(channel->get_name(), juce::dontSendNotification);
    }
    repaint();
}