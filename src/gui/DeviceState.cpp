#include "DeviceState.h"

DeviceState::DeviceState() {
    addAndMakeVisible(m_device_name);
}

DeviceState::~DeviceState() {
    m_src->removeChangeListener(this);
}

void DeviceState::add_as_listener(juce::ChangeBroadcaster* source) {
    source->addChangeListener(this);
    m_src = source;
}

void DeviceState::set_label(juce::String name) {
    m_device_name.setText(name, juce::dontSendNotification);
    m_device_name.setJustificationType(juce::Justification::centredRight);
    m_device_name.setFont(juce::FontOptions().withPointHeight(16));
}

void DeviceState::paint(juce::Graphics& g) {
    g.setColour(colormap[m_connect_status]);
    g.fillRect(130, getHeight() / 4, getHeight() / 2, getHeight() / 2);
}

void DeviceState::resized() {
    m_device_name.setBounds(0, 0, 120, getHeight());
}

void DeviceState::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == m_src) {
        m_connect_status = !m_connect_status;
        DBG("Connected? " << (m_connect_status ? "Y" : "N"));
    }
    repaint();
}