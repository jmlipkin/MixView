#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"
#include "../MessageProcessor.h"

struct AssignmentContent {
    const ChannelStrip* this_channel;
    juce::Label channel_id;
};

class AssignmentsPopout : public juce::Component {
   public:
    AssignmentsPopout(AssignmentContent* content, std::shared_ptr<MessageProcessor> processor);

    void setContent();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;

   private:
    bool inputIsAssigned(int index);
    bool auxIsAssigned(int index);
    bool busIsAssigned(int index);

   private:
    juce::Rectangle<int> m_contentArea;
    AssignmentContent* m_content;

    std::shared_ptr<MessageProcessor> mp;
    
    // Type labels
    juce::Label input;
    juce::Label aux;
    juce::Label bus;

    // Numbers
    juce::Label inputs[32];
    juce::Label auxes[8];
    juce::Label buses[16];

    // Draw parameters
    int line_width = 3;
    int corner_radius = 20;
    int bubble_size = 40;

    const juce::Colour CLR_BKG = juce::Colour(0xFF002538);
    const juce::Colour CLR_EDG = juce::Colour(0xFFFFFFFF);
    const juce::Colour CLR_OFF_CIRC = juce::Colour(0xFFFFFFFF);
    const juce::Colour CLR_ON_CIRC = juce::Colour(0xFFD1D654);
};