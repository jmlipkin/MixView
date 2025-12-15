#include "ScribbleStrip.h"

#include "../ChannelStrip.h"

ScribbleStrip::ScribbleStrip(ChannelStrip* ch, std::shared_ptr<MessageProcessor> processor) : channel(ch), mp(processor) {
    channel->scribble_broadcaster.addChangeListener(this);

    channel_id.setText(channel->get_id(), juce::dontSendNotification);
    channel_id.setBounds(0, 5, getWidth(), 20);
    channel_id.setJustificationType(juce::Justification::horizontallyCentred);
    channel_id.setInterceptsMouseClicks(false, false);
    channel_name.setText(channel->get_name(), juce::dontSendNotification);
    channel_name.setBounds(0, 25, getWidth(), 20);
    channel_name.setFont(juce::FontOptions().withHeight(19));
    channel_name.setJustificationType(juce::Justification::horizontallyCentred);
    channel_name.setInterceptsMouseClicks(false, false);

    m_content.this_channel = channel;
    m_content.channel_id.setText(channel->get_id(), juce::dontSendNotification);

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

void ScribbleStrip::resized() {
    channel_id.setBounds(0, 5, getWidth(), 20);
    channel_name.setBounds(0,35, getWidth(), 20);
}

void ScribbleStrip::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &channel->scribble_broadcaster) {
        channel_color = channel->get_color_value();
        channel_id.setText(channel->get_id(), juce::dontSendNotification);
        channel_name.setText(channel->get_name(), juce::dontSendNotification);
    }
    repaint();
}

void ScribbleStrip::mouseDown(const juce::MouseEvent& e) {
    DBG("Mouse down! " << channel_id.getText());

    juce::Component* mainComponent = getParentComponent()->getParentComponent();
    m_popout = std::make_unique<AssignmentsPopout>(&m_content, mp);
    m_popout->setContent();
    m_popout->setBounds(0, 0, mainComponent->getWidth(), mainComponent->getHeight());
    m_popout->setAlwaysOnTop(true);
    m_popout->setInterceptsMouseClicks(true, true);
    mainComponent->addAndMakeVisible(*m_popout);
}