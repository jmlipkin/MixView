#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class DeviceState : public juce::Component, public juce::ChangeListener {
   public:
    DeviceState();
    ~DeviceState() override;

    void add_as_listener(juce::ChangeBroadcaster* source);

    void set_connect_state(bool state) { m_connect_status = state; }
    
    void set_label(juce::String name);
    juce::Label* get_label() { return &m_device_name; }

    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

   private:
    juce::ChangeBroadcaster* m_src;

    juce::Label m_device_name;
    bool m_connect_status = false;

    std::vector<juce::Colour> colormap = {
        juce::Colour(0xFFFF0000),  // disconnected
        juce::Colour(0xFF00FF00)   // connected
    };
};