#include "MainComponent.h"

#include "Macros.h"
#include "OSCConnect.h"

#define IP_ADDRESS "10.5.227.58"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (int(590*1.75), int(410*1.75));
    getLookAndFeel().setDefaultSansSerifTypefaceName("Helvetica Neue");

    mp = connector.get_message_processor();
    strip_dca1 = std::make_unique<ScribbleStrip> (mp->get_dca(0));
    state_dca1 = std::make_unique<State>(mp->get_dca(0));
    fader_dca1 = std::make_unique<Fader>(mp->get_dca(0));

    strip_ch2 = std::make_unique<ScribbleStrip> (mp->get_in_ch(1));
    state_ch2 = std::make_unique<State>(mp->get_in_ch(1));

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

    addAndMakeVisible(strip_dca1.get());
    strip_dca1->setBounds(100, 200, 100, 50);
    addAndMakeVisible(state_dca1.get());
    state_dca1->setBounds(100, 275, 100, 25);
    addAndMakeVisible(fader_dca1.get());
    fader_dca1->setBounds(100, 325, 100, 300);

    addAndMakeVisible(strip_ch2.get());
    strip_ch2->setBounds(300, 400, 100, 50);
    addAndMakeVisible(state_ch2.get());
    state_ch2->setBounds(300, 475, 100, 25);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xFF00141B));

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