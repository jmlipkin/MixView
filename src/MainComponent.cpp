#include "MainComponent.h"

#include "macros.h"
#include "X32Connect.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);

    DBG("Program started" << DBG_STR);

    Xip_str.setText(connector.get_Xip(), juce::dontSendNotification);
    addAndMakeVisible(Xip_str);

    connector.set_Xip("127.0.0.1");
    connector.set_Xport(10023);

    juce::OSCAddressPattern adr("/info");
    juce::OSCMessage msg(adr);
    if (connector.sender.connect(connector.get_Xip(), connector.get_Xport()))
    {
        connector.sender.send(msg);
    }
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("hello tess :)", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    Xip_str.setBounds(200, 300, 100, 20);
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}