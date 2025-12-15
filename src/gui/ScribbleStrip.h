#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "../ChannelStrip.h"
#include "../MessageProcessor.h"

struct AssignmentContent {
    const ChannelStrip* this_channel;
    juce::Label channel_id;
};

class AssignmentsPopout : public juce::Component {
   private:
    juce::Rectangle<int> m_contentArea;
    AssignmentContent* m_content;

    std::shared_ptr<MessageProcessor> mp;

    const juce::Colour CLR_BKG = juce::Colour(0xFF002538);
    const juce::Colour CLR_EDG = juce::Colour(0xFFFFFFFF);
    const juce::Colour CLR_OFF_CIRC = juce::Colour(0xFFFFFFFF);
    const juce::Colour CLR_ON_CIRC = juce::Colour(0xFFD1D654);

    juce::Label inputs[32];
    juce::Label auxes[8];
    juce::Label buses[16];

    juce::Label input;
    juce::Label aux;
    juce::Label bus;

    int line_width = 3;
    int corner_radius = 20;

    int bubble_size = 40;

   public:
    AssignmentsPopout(AssignmentContent* content, std::shared_ptr<MessageProcessor> processor) : m_content(content), mp(processor) {
        DBG("New Popout created");

        for (int i = 0; i < 32; i++) {
            inputs[i].setText(juce::String(i + 1), juce::dontSendNotification);
            inputs[i].setJustificationType(juce::Justification::centred);
            addAndMakeVisible(inputs[i]);
        }
        for (int i = 0; i < 8; i++) {
            auxes[i].setText(juce::String(i + 1), juce::dontSendNotification);
            auxes[i].setJustificationType(juce::Justification::centred);
            addAndMakeVisible(auxes[i]);
        }
        for (int i = 0; i < 16; i++) {
            buses[i].setText(juce::String(i + 1), juce::dontSendNotification);
            buses[i].setJustificationType(juce::Justification::centred);
            addAndMakeVisible(buses[i]);
        }

        input.setText("Input Channels", juce::dontSendNotification);
        input.setJustificationType(juce::Justification::centredLeft);
        aux.setText("Auxes", juce::dontSendNotification);
        aux.setJustificationType(juce::Justification::centredLeft);
        bus.setText("Buses", juce::dontSendNotification);
        bus.setJustificationType(juce::Justification::centredLeft);

        addAndMakeVisible(input);
        addAndMakeVisible(aux);
        addAndMakeVisible(bus);
    }

    ~AssignmentsPopout() override { DBG("Popout destroyed"); }

    void setContent() {
        bool status = mp->get_in_ch(0)->get_dca_assignments()[m_content->this_channel->get_num_index()];

        addAndMakeVisible(m_content->channel_id);
        m_content->channel_id.setJustificationType(juce::Justification::centredTop);
        DBG("Content set: " << (status ? "1" : "0"));
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::transparentWhite);

        g.setColour(CLR_EDG);
        g.fillRoundedRectangle(m_contentArea.toFloat(), corner_radius);
        g.setColour(CLR_BKG);
        g.fillRoundedRectangle(m_contentArea.reduced(line_width, line_width).toFloat(), corner_radius - line_width);

        float x, y;
        x = m_contentArea.getTopLeft().x;
        y = m_contentArea.getTopLeft().y;

        int xoffset = 25;
        int padding = float(bubble_size) / 3.0f;
        int label_height = 15;
        int label_space = label_height + padding;

        for (int i = 0; i < 16; i++) {
            g.setColour((inputIsAssigned(i) ? CLR_ON_CIRC : CLR_OFF_CIRC));
            g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + label_space, bubble_size, bubble_size, 2);
        }
        for (int i = 0; i < 16; i++) {
            g.setColour((inputIsAssigned(i + 16) ? CLR_ON_CIRC : CLR_OFF_CIRC));
            g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + label_space + (bubble_size + padding), bubble_size, bubble_size, 2);
        }

        for (int i = 0; i < 8; i++) {
            g.setColour((auxIsAssigned(i) ? CLR_ON_CIRC : CLR_OFF_CIRC));
            g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + 2*label_space + 2 * (bubble_size + padding), bubble_size, bubble_size, 2);
        }
        for (int i = 0; i < 16; i++) {
            g.setColour((busIsAssigned(i) ? CLR_ON_CIRC : CLR_OFF_CIRC));
            g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + 3*label_space + (3 * (bubble_size + padding)), bubble_size, bubble_size, 2);
        }
    }

    void resized() override {
        m_contentArea = {getWidth() / 8, getHeight() / 4, getWidth() * 3 / 4, getHeight() / 2};
        m_content->channel_id.setBounds(m_contentArea.reduced(line_width, line_width));

        bubble_size = getWidth() / 30;

        int x, y;
        x = m_contentArea.getTopLeft().x;
        y = m_contentArea.getTopLeft().y;
        int xoffset = 25;
        int padding = float(bubble_size) / 3.0f;
        int label_height = 15;
        int label_space = label_height + padding;

        input.setBounds(x + xoffset, y + 40, 120, label_height);
        aux.setBounds(x + xoffset, y + 40 + label_space + 2 * (bubble_size + padding), 120, label_height);
        bus.setBounds(x + xoffset, y + 40 + 2 * label_space + 3 * (bubble_size + padding), 120, label_height);


        for (int i = 0; i < 16; i++) {
            inputs[i].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + label_space, bubble_size, bubble_size);
        }
        for (int i = 0; i < 16; i++) {
            inputs[i + 16].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + label_space + (bubble_size + padding), bubble_size, bubble_size);
        }

        for (int i = 0; i < 8; i++) {
            auxes[i].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + 2*label_space + 2 * (bubble_size + padding), bubble_size, bubble_size);
        }
        for (int i = 0; i < 16; i++) {
            buses[i].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + 3*label_space + (3 * (bubble_size + padding)), bubble_size, bubble_size);
        }
    }

    void mouseDown(const juce::MouseEvent& e) override {
        setVisible(false);
        getParentComponent()->removeChildComponent(this);
    }

    private:
    bool inputIsAssigned(size_t index) {
        return mp->get_in_ch(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
    }

    bool auxIsAssigned(size_t index) {
        return mp->get_auxin(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
    }

    bool busIsAssigned(size_t index) {
        return mp->get_bus(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
    }
};

class ScribbleStrip : public juce::Component, public juce::ChangeListener {
   private:
    juce::Colour C_OFF = juce::Colour(0xFF001923);
    //  juce::Colour C_RD;
    juce::Colour C_GN = juce::Colour(0xFF006D36);
    juce::Colour C_YE;
    juce::Colour C_BL;
    juce::Colour C_MG;
    juce::Colour C_CY;
    juce::Colour C_WH = juce::Colour(0xFF002538);
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //  juce::Colour C_GN;
    //      juce::Colour C_WHi = juce::Colour();

    const std::vector<juce::Colour> SCRIBBLE_BKG = {
        {juce::Colour(0xFF001923)},  // off
        {juce::Colour(0x00000000)},  // red
        {juce::Colour(0xFF055728)},  // green
        {juce::Colour(0xFF767019)},  // yellow
        {juce::Colour(0xFF002538)},  // blues
        {juce::Colour(0xFF4A2A67)},  // magenta
        {juce::Colour(0xFF00707C)},  // cyan
        {juce::Colour(0xFF002538)},  // white
        {juce::Colour(0x00000000)},  // offi
        {juce::Colour(0x00000000)},  // redi
        {juce::Colour(0xFF2E8C52)},  // greeni
        {juce::Colour(0xFF767019)},  // yellowi
        {juce::Colour(0x00000000)},  // bluei
        {juce::Colour(0xFF693398)},  // magentai
        {juce::Colour(0xFF59A9B4)},  // cyani
        {juce::Colour(0xFF324855)}   // whitei

    };
    const std::vector<juce::Colour> SCRIBBLE_EDGE = {
        {juce::Colour(0xFF002436)},  // off
        {juce::Colour(0x00000000)},  // red
        {juce::Colour(0xFF89FF96)},  // green
        {juce::Colour(0xFFD1D654)},  // yellow
        {juce::Colour(0xFF0083B8)},  // blue
        {juce::Colour(0xFFD92BF9)},  // magenta
        {juce::Colour(0xFF00F2F5)},  // cyan
        {juce::Colour(0xFFFFFFFF)},  // white
        {juce::Colour(0x00000000)},  // offi
        {juce::Colour(0x00000000)},  // redi
        {juce::Colour(0xFF89FF96)},  // greeni
        {juce::Colour(0xFFD1D654)},  // yellowi
        {juce::Colour(0x00000000)},  // bluei
        {juce::Colour(0xFFD92BF9)},  // magentai
        {juce::Colour(0x00000000)},  // cyani
        {juce::Colour(0xFFBBBBBB)}   // whitei
    };

    ChannelStrip* channel = nullptr;
    juce::Label channel_id;
    juce::Label channel_name;
    ChannelStrip::COLORS channel_color;
    std::unique_ptr<AssignmentsPopout> m_popout;
    AssignmentContent m_content;
    std::shared_ptr<MessageProcessor> mp;

   public:
    ScribbleStrip(ChannelStrip* ch, std::shared_ptr<MessageProcessor> processor);
    ~ScribbleStrip() override;
    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void mouseDown(const juce::MouseEvent& e) override;
};