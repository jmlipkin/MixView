#include "MessageProcessor.h"

MessageProcessor::MessageProcessor() : juce::Thread("MessageProcess") {
    m_channels = new InputChannelStrip[NUM_IN_CH];
    m_auxins = new InputChannelStrip[NUM_AUXINS];
    m_buses = new InputChannelStrip[NUM_BUSES];
    m_matrices = new ChannelStrip[NUM_MATRICES];
    m_main_st = new ChannelStrip;
    m_dcas = new DCAChannelStrip[NUM_DCAS];
    m_tmix = new TMixProcessor;

    for (size_t i = 0; i < NUM_IN_CH; i++) {
        m_channels[i].set_id("CH " + juce::String(i + 1));
        m_channels[i].set_num_and_ap(i);
    }
    for (size_t i = 0; i < NUM_AUXINS; i++) {
        m_auxins[i].set_id("AUX " + juce::String(i + 1));
        m_auxins[i].set_num_and_ap(i);
    }
    for (size_t i = 0; i < NUM_BUSES; i++) {
        m_buses[i].set_id("BUS " + juce::String(i + 1));
        m_buses[i].set_num_and_ap(i);
    }
    for (size_t i = 0; i < NUM_MATRICES; i++) {
        m_matrices[i].set_id("MTX " + juce::String(i + 1));
        m_matrices[i].set_num_and_ap(i);
    }
    m_main_st->set_id("LR");
    for (size_t i = 0; i < NUM_DCAS; i++) {
        m_dcas[i].set_id("DCA " + juce::String(i + 1));
        m_dcas[i].set_num_and_ap(i);
    }

    last_thump_time.store(std::chrono::high_resolution_clock::now());
}
MessageProcessor::~MessageProcessor() {
    delete[] m_channels;
    delete[] m_auxins;
    delete[] m_buses;
    delete[] m_matrices;
    delete[] m_dcas;
    delete m_main_st;
    delete m_tmix;

    m_channels = nullptr;
    m_auxins = nullptr;
    m_buses = nullptr;
    m_matrices = nullptr;
    m_dcas = nullptr;
    m_main_st = nullptr;
    m_tmix = nullptr;

    stop();
}

void MessageProcessor::add_to_buffer(juce::OSCMessage message) {
    std::lock_guard<std::mutex> lock(mutex);
    m_msg_buffer.push_back(message);

    messageReady.signal();

    if (!isThreadRunning())
        startThread();
}

void MessageProcessor::run() {
    while (!threadShouldExit()) {
        messageReady.wait(-1);

        while (true) {
            juce::OSCMessage message{"/null_from_mp_thread"};
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (m_msg_buffer.empty()) {
                    break;
                }
                message = std::move(m_msg_buffer.front());
                m_msg_buffer.pop_front();
            }

            process_message(message);
        }
    }
}

void MessageProcessor::stop() {
    signalThreadShouldExit();
    messageReady.signal();
    stopThread(1000);
}

void MessageProcessor::process_message(juce::OSCMessage& message) {
    size_t idx = 100;
    switch (find_channel_type(message)) {
        case ChannelStrip::CH_TYPE::IN_CH: {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::IN_CH);
            m_channels[idx].update_parameter(message);

            break;
        }
        case ChannelStrip::CH_TYPE::AUX_IN: {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::AUX_IN);
            m_auxins[idx].update_parameter(message);

            break;
        }
        case ChannelStrip::CH_TYPE::BUS: {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::BUS);
            m_buses[idx].update_parameter(message);

            break;
        }
        case ChannelStrip::CH_TYPE::DCA: {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::DCA);
            m_dcas[idx].update_parameter(message);

            break;
        }
        case ChannelStrip::CH_TYPE::MATRIX: {
            idx = find_channel_idx(message, ChannelStrip::CH_TYPE::MATRIX);
            m_matrices[idx].update_parameter(message);
            break;
        }
        case ChannelStrip::CH_TYPE::MAIN_ST: {
            m_main_st->update_parameter(message);
            break;
        }
        case ChannelStrip::CH_TYPE::INVALID: {
            if (m_tmix->is_type_tmix(message)) {
                m_tmix->process_message(message);
                break;
            }
            print_unprocessed_string(message);
            break;
        }
        case ChannelStrip::CH_TYPE::INFO: {
            last_thump_time.store(std::chrono::high_resolution_clock::now());
            break;
        }
        default:
            print_unprocessed_string(message);
            break;
    }
}

void MessageProcessor::print_unprocessed_string(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();
    int num_args = message.size();
    juce::String msg_str = address;

    for (int i = 0; i < num_args; i++) {
        msg_str += " " + arg_to_str(message[i]);
    }

    DBG(msg_str);
}

ChannelStrip::CH_TYPE MessageProcessor::find_channel_type(juce::OSCMessage& message) {
    juce::String address = message.getAddressPattern().toString();

    for (size_t i = 0; i < ChannelStrip::channel_type.size(); i++) {
        if (address.startsWith(ChannelStrip::channel_type[i])) {
            // leave message alone if type info
            if (i == ChannelStrip::CH_TYPE::INFO) {
                return static_cast<ChannelStrip::CH_TYPE>(i);
            }
            address = address.substring(ChannelStrip::channel_type[i].length());
            message.setAddressPattern(address);
            return static_cast<ChannelStrip::CH_TYPE>(i);
        }
    }

    return ChannelStrip::INVALID;
}

size_t MessageProcessor::find_channel_idx(juce::OSCMessage& message, ChannelStrip::CH_TYPE type) {
    juce::String address = message.getAddressPattern().toString();

    switch (type) {
        case ChannelStrip::IN_CH:
            for (size_t i = 0; i < NUM_IN_CH; i++) {
                if (address.startsWith(m_channels[i].get_num_ap().toString())) {
                    address = address.substring(m_channels[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
            break;
        case ChannelStrip::CH_TYPE::AUX_IN: {
            for (size_t i = 0; i < NUM_AUXINS; i++) {
                if (address.startsWith(m_auxins[i].get_num_ap().toString())) {
                    address = address.substring(m_auxins[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::BUS: {
            for (size_t i = 0; i < NUM_BUSES; i++) {
                if (address.startsWith(m_buses[i].get_num_ap().toString())) {
                    address = address.substring(m_buses[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::DCA: {
            for (size_t i = 0; i < NUM_DCAS; i++) {
                if (address.startsWith(m_dcas[i].get_num_ap().toString())) {
                    address = address.substring(m_dcas[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::MATRIX: {
            for (size_t i = 0; i < NUM_MATRICES; i++) {
                if (address.startsWith(m_matrices[i].get_num_ap().toString())) {
                    address = address.substring(m_matrices[i].get_num_ap().toString().length());
                    message.setAddressPattern(address);
                    return i;
                }
            }
        }
        case ChannelStrip::CH_TYPE::MAIN_ST: {
            break;
        }
        case ChannelStrip::CH_TYPE::INFO: {
            break;
        }
        case ChannelStrip::CH_TYPE::INVALID:
            return 100;
        default:
            return 100;
    }
    return 100;
}

juce::String MessageProcessor::arg_to_str(const juce::OSCArgument& arg) {
    switch (arg.getType()) {
        case 'f':
            return juce::String(arg.getFloat32());
        case 'i':
            return juce::String(arg.getInt32());
        case 's':
            return arg.getString();

        default:
            return juce::String(" + 1TYPE NOT HANDLED");
            break;
    }
}