#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../OSCConnect.h"

class InfoBar : public juce::Component {
   private:
    IPAddressBox ip_str_X32;
    IPAddressBox ip_str_tmix;
    IPAddressBox ip_str_this;

    std::unique_ptr<juce::IPAddress> ip_X32;
    std::unique_ptr<juce::IPAddress> ip_tmix;
    std::unique_ptr<juce::IPAddress> ip_this;

    juce::Label label_X32;
    juce::Label label_tmix;
    juce::Label label_this;

    juce::TextButton starter;

    OSCConnect& connector;

   public:
    InfoBar(OSCConnect& connect) : connector(connect) {
        ip_X32 = std::make_unique<juce::IPAddress>(connector.get_ip_X32());
        ip_tmix = std::make_unique<juce::IPAddress>(connector.get_ip_tmix());
        ip_this = std::make_unique<juce::IPAddress>(connector.get_ip_this());

        ip_str_X32.setIPAddress(connector.get_ip_X32());
        addAndMakeVisible(ip_str_X32);

        ip_str_tmix.setIPAddress(connector.get_ip_tmix());
        addAndMakeVisible(ip_str_tmix);

        ip_str_this.setIPAddress(connector.get_ip_this());
        addAndMakeVisible(ip_str_this);

        ip_str_X32.onEditorHide = [this] { update_ip_address(ip_str_X32, ip_X32); };
        ip_str_tmix.onEditorHide = [this] { update_ip_address(ip_str_tmix, ip_tmix); };
        ip_str_this.onEditorHide = [this] { update_ip_address(ip_str_this, ip_this); };

        starter.setButtonText("CONNECT");
        starter.setToggleable(true);
        starter.setToggleState(true, juce::dontSendNotification);
        starter.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF02141A));
        starter.onClick = [this] { startButtonClicked(); };
        addAndMakeVisible(starter);
    }

    void paint(juce::Graphics& g) override {
        int corner_radius = 15;
        g.setColour(juce::Colour(0xFF002436));
        g.fillRoundedRectangle(0, -10, getWidth(), getHeight() + 10, corner_radius);
    }

    void resized() override {
        ip_str_this.setBounds(200, getHeight() * 1 / 2, 200, 18);
        ip_str_X32.setBounds(450, 10, 200, 18);
        ip_str_tmix.setBounds(450, 30, 200, 18);
        starter.setBounds(10, getHeight() / 4, 100, getHeight() * 1 / 2);
    }

    void update_ip_address(IPAddressBox& ip_str, std::unique_ptr<juce::IPAddress>& addr) {
        if (ip_str.isChanged()) {
            addr = std::make_unique<juce::IPAddress>(ip_str.getText());
            DBG("IP address changed to " << ip_str.getText());
        }
    }

    void startButtonClicked() {
        if (starter.getToggleState()) {
            starter.setButtonText("DISCONNECT");
            OSCConnect::Connected_State status = connector.connect();
            if (status.X32) {
                connector.open();
            }

            ip_str_this.setEditable(false);
            ip_str_tmix.setEditable(false);
            ip_str_X32.setEditable(false);

        } else {
            starter.setButtonText("STOPPING...");
            connector.close(-1);

            DBG("Connector stopped thread" << DBG_STR);
            starter.setButtonText("CONNECT");

            ip_str_this.setEditable(true);
            ip_str_tmix.setEditable(true);
            ip_str_X32.setEditable(true);
        }
        starter.setToggleState(!starter.getToggleState(), juce::dontSendNotification);
    }
};
