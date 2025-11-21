#include "MainComponent.h"

#include "Macros.h"
#include "OSCConnect.h"

#define CHANNEL_WIDTH 90

//==============================================================================
MainComponent::MainComponent() : mp(std::make_unique<MessageProcessor>())
{
    setSize (int(590*1.75), int(410*1.75));
    getLookAndFeel().setDefaultSansSerifTypefaceName("Helvetica Neue");

    // TODO: Move to new class?
    initialize_strips();

    // TMix initialization
    tmix = std::make_unique<TMixView>(mp->get_tmix());
    addAndMakeVisible(tmix.get());
    tmix->setBounds(getWidth() / 3, 80, getWidth() / 3, 40);

    // Menu initialization
    menu = std::make_unique<InfoBar>(*mp);
    addAndMakeVisible(menu.get());
    menu->setBounds(0, 0, getWidth(), 50);

    DBG("Program started" << DBG_STR);
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

void MainComponent::initialize_strips()
{
    for (size_t i = 0; i < 8; i++)
    {
        dca_strips.push_back(std::make_unique<StripView>(mp->get_dca(int(i))));
        addAndMakeVisible(dca_strips[i].get());
        dca_strips[i]->setBounds(int(i) * (CHANNEL_WIDTH + 20) + 20, getHeight() - 550 - 20, CHANNEL_WIDTH, 550);
    }
    lr_strip = std::make_unique<StripView>(mp->get_main_st());
    addAndMakeVisible(lr_strip.get());
    lr_strip->setBounds(getWidth() - 30 - CHANNEL_WIDTH, getHeight() - 550 - 20, CHANNEL_WIDTH, 550);
}