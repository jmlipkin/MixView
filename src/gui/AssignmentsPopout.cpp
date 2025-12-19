#include "AssignmentsPopout.h"

AssignmentsPopout::AssignmentsPopout(AssignmentContent* content, std::shared_ptr<MessageProcessor> processor) : m_content(content), mp(processor) {
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

void AssignmentsPopout::setContent() {
    bool status = mp->get_in_ch(0)->get_dca_assignments()[m_content->this_channel->get_num_index()];

    addAndMakeVisible(m_content->channel_id);
    m_content->channel_id.setJustificationType(juce::Justification::centredTop);
    DBG("Content set: " << (status ? "1" : "0"));
}

void AssignmentsPopout::paint(juce::Graphics& g) {
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
        g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + 2 * label_space + 2 * (bubble_size + padding), bubble_size, bubble_size, 2);
    }
    for (int i = 0; i < 16; i++) {
        g.setColour((busIsAssigned(i) ? CLR_ON_CIRC : CLR_OFF_CIRC));
        g.drawEllipse(x + xoffset + i * (padding + bubble_size), y + 40 + 3 * label_space + (3 * (bubble_size + padding)), bubble_size, bubble_size, 2);
    }
}

void AssignmentsPopout::resized() {
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
        auxes[i].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + 2 * label_space + 2 * (bubble_size + padding), bubble_size, bubble_size);
    }
    for (int i = 0; i < 16; i++) {
        buses[i].setBounds(x + xoffset + i * (padding + bubble_size), y + 40 + 3 * label_space + (3 * (bubble_size + padding)), bubble_size, bubble_size);
    }
}

void AssignmentsPopout::mouseDown(const juce::MouseEvent& e) {
    setVisible(false);
    getParentComponent()->removeChildComponent(this);
}

bool AssignmentsPopout::inputIsAssigned(int index) {
    return mp->get_in_ch(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
}

bool AssignmentsPopout::auxIsAssigned(int index) {
    return mp->get_auxin(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
}

bool AssignmentsPopout::busIsAssigned(int index) {
    return mp->get_bus(index)->get_dca_assignments()[m_content->this_channel->get_num_index()];
}