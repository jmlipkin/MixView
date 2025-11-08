#include "MainComponent.h"

#include "Macros.h"
#include "OSCConnect.h"

#define IP_ADDRESS "10.5.227.58"
#define CHANNEL_WIDTH 90

//==============================================================================
MainComponent::MainComponent()
{
    setSize (int(590*1.75), int(410*1.75));
    getLookAndFeel().setDefaultSansSerifTypefaceName("Helvetica Neue");

    mp = connector.get_message_processor();
    for (size_t i = 0; i < 8; i++)
    {
        dca_strips.push_back(std::make_unique<StripView>(mp->get_dca(i)));
        addAndMakeVisible(dca_strips[i].get());
        dca_strips[i]->setBounds(i * (CHANNEL_WIDTH + 20) + 20, getHeight() - 550 - 20, CHANNEL_WIDTH, 550);
    }
    lr_strip = std::make_unique<StripView>(mp->get_main_st());
    addAndMakeVisible(lr_strip.get());
    lr_strip->setBounds(getWidth() - 30 - CHANNEL_WIDTH, getHeight() - 550 - 20, CHANNEL_WIDTH, 550);

    tmix = std::make_unique<TMixView>(mp->get_tmix());
    addAndMakeVisible(tmix.get());
    tmix->setBounds(getWidth() / 3, 80, getWidth() / 3, 40);

    connector.set_ip_this(IP_ADDRESS);
    connector.set_ip_tmix(IP_ADDRESS);
    connector.set_ip_X32(IP_ADDRESS);

    menu = std::make_unique<InfoBar>(connector);
    addAndMakeVisible(menu.get());
    menu->setBounds(0, 0, getWidth(), 50);

    DBG("Program started" << DBG_STR);


    connector.set_timeout(1000);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xFF00141B));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}