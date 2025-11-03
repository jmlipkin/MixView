#include "MainComponent.h"

#include "Macros.h"
#include "X32Connect.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);

    DBG("Program started" << DBG_STR);

    Xip_str.setText(connector.get_Xip(), juce::dontSendNotification);
    addAndMakeVisible(Xip_str);

    starter.setButtonText("Start");
    starter.setToggleable(true);
    starter.setToggleState(true, juce::dontSendNotification);
    starter.onClick = [this]
    { startButtonClicked(); };
    addAndMakeVisible(starter);

    connector.set_timeout(5000);
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
    Xip_str.setBounds(100, 10, 100, 20);
    starter.setBounds(10, 10, 60, 20);
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::startButtonClicked(){
    if (starter.getToggleState())
    {
        starter.setButtonText("Running");
        if (connector.connect("127.0.0.1", 10023))
        {
            connector.startThread();
            DBG("Connector started thread" << DBG_STR);

            connector.sender.get_info_X32();
        }

        int c = 0;
        while (c < 500)
        {
            DBG("main thread " << c++);
            juce::Thread::sleep(100);
        }
    }
    else {
        starter.setButtonText("Stopping...");
        connector.stopThread(-1);

        DBG("Connector stopped thread" << DBG_STR);
        starter.setButtonText("Start");
    }
    starter.setToggleState(!starter.getToggleState(), juce::dontSendNotification);
}