#pragma once

class DeviceState : public juce::Component, public juce::ChangeListener {
    private:
     std::vector<juce::Colour> colormap = {
        juce::Colour(0xFFFF0000), // disconnected
        juce::Colour(0xFF00FF00) // connected
        }
     ;
     juce::Label m_device_name;
     bool m_connect_status = false;

     juce::ChangeBroadcaster* m_src;

    public:
    DeviceState() {
        addAndMakeVisible(m_device_name);
    }
    ~DeviceState() override {
        m_src->removeChangeListener(this);
    }

    void add_as_listener(juce::ChangeBroadcaster* source) {
        source->addChangeListener(this);
        m_src = source;
    }

    void set_label(juce::String name) {
        m_device_name.setText(name, juce::dontSendNotification);
        m_device_name.setJustificationType(juce::Justification::centredRight);
        m_device_name.setFont(juce::FontOptions().withPointHeight(16));
    }

    void paint(juce::Graphics& g) override {
        g.setColour(colormap[m_connect_status]);
        g.fillRect(130, getHeight() / 2, getHeight()/2, getHeight()/2);
    }

    void resized() override {

    }

    void set_connect_state(bool state) { m_connect_status = state; }

    juce::Label* get_label() { return &m_device_name; }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {
        if (source == m_src) {
            m_connect_status = !m_connect_status;
            DBG("Connected? " << (m_connect_status ? "Y" : "N"));
        }
        repaint();
     }
};