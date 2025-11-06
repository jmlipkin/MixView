#include "MainComponent.h"

#include "Macros.h"
#include "OSCConnect.h"

#define IP_ADDRESS "127.0.0.1"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (590, 410);

    DBG("Program started" << DBG_STR);

    connector.set_ip_this(IP_ADDRESS);
    connector.set_ip_tmix(IP_ADDRESS);
    connector.set_ip_X32(IP_ADDRESS);

    ip_str_X32.setText("X32: " + connector.get_ip_X32(), juce::dontSendNotification);
    addAndMakeVisible(ip_str_X32);
    ip_str_tmix.setText("TMix: " + connector.get_ip_tmix(), juce::dontSendNotification);
    addAndMakeVisible(ip_str_tmix);
    ip_str_this.setText("This: " + connector.get_ip_this(), juce::dontSendNotification);
    addAndMakeVisible(ip_str_this);

    starter.setButtonText("Start");
    starter.setToggleable(true);
    starter.setToggleState(true, juce::dontSendNotification);
    starter.onClick = [this]
    { startButtonClicked(); };
    addAndMakeVisible(starter);

    connector.set_timeout(1000);
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
    ip_str_X32.setBounds(100, 10, 100, 20);
    ip_str_this.setBounds(200, 10, 100, 20);
    ip_str_tmix.setBounds(300, 10, 100, 20);
    starter.setBounds(10, 10, 60, 20);
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::startButtonClicked(){
    if (starter.getToggleState())
    {
        starter.setButtonText("Running");
        OSCConnect::Connected_State status = connector.connect();
        if (status.X32) {
            connector.open();
        }

        // MAKE_BUSY();
    }
    else {
        starter.setButtonText("Stopping...");
        connector.close(-1);

        DBG("Connector stopped thread" << DBG_STR);
        starter.setButtonText("Start");
    }
    starter.setToggleState(!starter.getToggleState(), juce::dontSendNotification);
}