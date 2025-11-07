#include "ChannelStrip.h"

void ChannelStrip::update_parameter(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();

    if (num_args == 1) {
        if (address.compare(name.ap.toString()) == 0) {
            set_name(message[0].getString());
            DBG(id << " name set to " << get_name());
            scribble_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(color.ap.toString()) == 0) {
            set_color_value(message[0].getInt32());
            DBG(id << " color set to " << color.toString());
            scribble_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(state.ap.toString()) == 0) {
            set_state_value(message[0].getInt32());
            DBG(id << " on state set to " << get_state_string());
            state_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(fader.ap.toString()) == 0) {
            set_fader_value(message[0].getFloat32());
            DBG(id << " fader set to " << get_fader_value());
            fader_broadcaster.sendChangeMessage();
            return;
        }
        DBG("Unknown address pattern from OSC message. Detected address: [" << address << "] (DBG printed from " << __FILE__ << " : " << __LINE__ << ")");
    }
    DBG("ERROR: Unexpected number of arguments attached to message " << unprocessed_string(message) << "]. DBG printed from " << __FILE__ << " : " << __LINE__);
}

void InputChannelStrip::update_parameter(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();

    if (num_args == 1) {
        if (address.compare(name.ap.toString()) == 0) {
            set_name(message[0].getString());
            DBG(id << " name set to " << get_name());
            scribble_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(color.ap.toString()) == 0) {
            set_color_value(message[0].getInt32());
            DBG(id << " color set to " << color.toString());
            scribble_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(state.ap.toString()) == 0) {
            set_state_value(message[0].getInt32());
            DBG(id << " on state set to " << get_state_string());
            state_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(fader.ap.toString()) == 0) {
            set_fader_value(message[0].getFloat32());
            DBG(id << " fader set to " << get_fader_value());
            fader_broadcaster.sendChangeMessage();
            return;
        }
        if (address.compare(dca_assignments.ap.toString()) == 0) {
            DBG("DCA assignment change");
            set_dca_assignments(message[0].getInt32());
            DBG(id << " DCA message: " << get_dca_assignments());
            dca_broadcaster.sendChangeMessage();
            return;
        }
        DBG("Unknown address pattern from OSC message. Detected address: [" << address << "] (DBG printed from " << __FILE__ << " : " << __LINE__ << ")");
    }
    DBG("ERROR: Unexpected number of arguments attached to message " << unprocessed_string(message) << "]. DBG printed from " << __FILE__ << " : " << __LINE__);
}

void ChannelStrip::set_num_and_ap(size_t idx) {
    if (idx + 1 < 10)
        number.ap = "/0" + juce::String(idx + 1);
    else
        number.ap = "/" + juce::String(idx + 1);
    number.index = idx;
}

void DCAChannelStrip::set_num_and_ap(size_t idx) {
    number.ap = "/" + juce::String(idx + 1);
    number.index = idx;
}

void ChannelStrip::print_string_with_ch_id(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();
    juce::String msg_str = address;

    for (int i = 0; i < num_args; i++) {
        msg_str += " " + arg_to_str(message[i]);
    }

    DBG(name.value << " " << number.index + 1 << ": " << msg_str);
}

juce::String ChannelStrip::arg_to_str(const juce::OSCArgument& arg) {
    switch (arg.getType()) {
        case 'f':
            return juce::String(arg.getFloat32());
        case 'i':
            return juce::String(arg.getInt32());
        case 's':
            return arg.getString();

        default:
            return juce::String("TYPE NOT HANDLED");
            break;
    }
}

juce::String ChannelStrip::unprocessed_string(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();
    juce::String msg_str = address;

    for (int i = 0; i < num_args; i++) {
        msg_str += " " + arg_to_str(message[i]);
    }

    return msg_str;
}

const std::vector<juce::String> ChannelStrip::channel_type = {"/ch", "/auxin", "/bus", "/mtx", "/dca"};