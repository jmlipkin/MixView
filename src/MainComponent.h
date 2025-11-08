#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "OSCConnect.h"

#include "gui/StripView.h"
#include "gui/TMixView.h"
// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent final : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override { connector.close(-1); }

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void startButtonClicked();

private:
    //==============================================================================
    // Your private member variables go here...
    OSCConnect connector;
    MessageProcessor* mp;

    juce::Label ip_str_X32;
    juce::Label ip_str_tmix;
    juce::Label ip_str_this;
    juce::TextButton starter;

    std::vector<std::unique_ptr<StripView>> dca_strips;
    std::unique_ptr<StripView> lr_strip;
    std::unique_ptr<TMixView> tmix;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};