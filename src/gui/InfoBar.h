#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../IPAddressBox.h"

class InfoBar : public juce::Component {
   private:
    IPAddressBox ip_console;
    IPAddressBox ip_tmix;
    IPAddressBox ip_host;

    juce::TextButton starter;

    OSCConnect connector;

   public:
    InfoBar(MessageProcessor& mp) : connector(mp, ip_host.getIPAddress(), ip_console.getIPAddress(), ip_tmix.getIPAddress()) {
        addAndMakeVisible(ip_host);
        addAndMakeVisible(ip_console);
        addAndMakeVisible(ip_tmix);

        ip_host.onEditorHide = [this] {
            ip_host.update_ip();
            if (ip_host.isChanged()) {
                connector.set_ip_host(ip_host.getIPAddress());
                connector.bind_all_ports();
                ip_host.set_isChanged(false);
            }
        };
        ip_console.onEditorHide = [this] {
            ip_console.update_ip();
            if (ip_console.isChanged()) {
                connector.set_ip_console(ip_console.getIPAddress());
                connector.bind_socket_X32();
                ip_console.set_isChanged(false);
            }
        };
        ip_tmix.onEditorHide = [this] {
            ip_tmix.update_ip();
            if (ip_tmix.isChanged()) {
                connector.set_ip_tmix(ip_tmix.getIPAddress());
                connector.bind_socket_tmix();
                ip_tmix.set_isChanged(false);
            }
        };

        starter.setButtonText("CONNECT");
        starter.setToggleable(true);
        starter.setToggleState(true, juce::dontSendNotification);
        starter.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF02141A));
        starter.onClick = [this] { startButtonClicked(); };
        addAndMakeVisible(starter);




        connector.set_timeout(1000);
    }

    ~InfoBar() override { connector.close(-1); }

    void paint(juce::Graphics& g) override {
        int corner_radius = 15;
        g.setColour(juce::Colour(0xFF002436));
        g.fillRoundedRectangle(0, -10, getWidth(), getHeight() + 10, corner_radius);
    }

    void resized() override {
        ip_host.setBounds(200, getHeight() * 1 / 2, 200, 18);
        ip_console.setBounds(450, 10, 200, 18);
        ip_tmix.setBounds(450, 30, 200, 18);
        starter.setBounds(10, getHeight() / 4, 100, getHeight() * 1 / 2);
    }

    void startButtonClicked() {
        if (starter.getToggleState()) {
            starter.setButtonText("DISCONNECT");

            connector.open();

            ip_host.setEditable(false);
            ip_tmix.setEditable(false);
            ip_console.setEditable(false);

        } else {
            starter.setButtonText("STOPPING...");
            connector.close(-1);

            DBG("Connector stopped thread" << DBG_STR);
            starter.setButtonText("CONNECT");

            ip_host.setEditable(true);
            ip_tmix.setEditable(true);
            ip_console.setEditable(true);
        }
        starter.setToggleState(!starter.getToggleState(), juce::dontSendNotification);
    }
};
