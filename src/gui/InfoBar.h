#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../OSCConnect.h"

class InfoBar : public juce::Component {
   private:
    juce::Label ip_str_X32;
    juce::Label ip_str_tmix;
    juce::Label ip_str_this;
    juce::TextButton starter;

    OSCConnect& connector;

   public:
    InfoBar(OSCConnect& connect) : connector(connect) {
        ip_str_X32.setText("X32: " + connector.get_ip_X32(), juce::dontSendNotification);
        addAndMakeVisible(ip_str_X32);

        ip_str_tmix.setText("TMix: " + connector.get_ip_tmix(), juce::dontSendNotification);
        addAndMakeVisible(ip_str_tmix);
        
        ip_str_this.setText("This: " + connector.get_ip_this(), juce::dontSendNotification);
        addAndMakeVisible(ip_str_this);

        starter.setButtonText("CONNECT");
        starter.setToggleable(true);
        starter.setToggleState(true, juce::dontSendNotification);
        starter.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF02141A));
        starter.onClick = [this] { startButtonClicked(); };
        addAndMakeVisible(starter);
    }

    void paint(juce::Graphics &g) override {
        int corner_radius = 15;
        g.setColour(juce::Colour(0xFF002436));
        g.fillRoundedRectangle(0, -10, getWidth(), getHeight() + 10, corner_radius);
    }

    void resized() override {
        ip_str_this.setBounds(200, getHeight()*1/2, 100, getHeight()/5);
        ip_str_X32.setBounds(450, 10, 100, 10);
        ip_str_tmix.setBounds(450, 30, 100, 20);
        starter.setBounds(10, getHeight() / 4, 100, getHeight() * 1/2);
    }

    void startButtonClicked() {
        if (starter.getToggleState()) {
            starter.setButtonText("DISCONNECT");
            OSCConnect::Connected_State status = connector.connect();
            if (status.X32) {
                connector.open();
            }

            // MAKE_BUSY();
        } else {
            starter.setButtonText("STOPPING...");
            connector.close(-1);

            DBG("Connector stopped thread" << DBG_STR);
            starter.setButtonText("CONNECT");
        }
        starter.setToggleState(!starter.getToggleState(), juce::dontSendNotification);
    }
};
