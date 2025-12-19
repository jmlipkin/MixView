#pragma once

#include <assert.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

#include <bitset>
#include <map>

inline const std::map<float, float>& get_fader_map();

class ChannelStrip {
   public:
    typedef enum COLORS {
        OFF,
        RD,
        GN,
        YE,
        BL,
        MG,
        CY,
        WH,
        OFFi,
        RDi,
        GNi,
        YEi,
        BLi,
        MGi,
        CYi,
        WHi
    } COLORS;

    typedef enum CH_TYPE {
        IN_CH,
        AUX_IN,
        BUS,  // Input channel
        MATRIX,
        MAIN_ST,  // Default channel
        DCA,      // DCA channel
        INFO,
        INVALID  // Auxiliary [not a channel]
    } CH_TYPE;

    typedef enum STATE {
        CH_OFF,
        CH_ON
    } STATE;

    static const std::vector<juce::String> channel_type;

   protected:
    ////////////////////////////////////
    // Attributes
    typedef struct Color {
        juce::OSCAddress ap = "/config/color";
        COLORS value;

        juce::String toString() const {
            static const char* names[] = {
                "OFF", "RD", "GN", "YE", "BL", "MG", "CY", "WH",
                "OFFi", "RDi", "GNi", "YEi", "BLi", "MGi", "CYi", "WHi"};
            return names[value];
        }
    } Color;

    typedef struct Name {
        juce::OSCAddress ap = "/config/name";
        juce::String value = "";

    } Name;

    typedef struct OnState {
        juce::OSCAddress ap = "/mix/on";
        STATE value = CH_OFF;
    } OnState;

    typedef struct Number {
        juce::OSCAddress ap = "/00";
        size_t index;
    } Number;

    typedef struct Fader {
        juce::OSCAddress ap = "/mix/fader";
        float value = 0.0;
        juce::String label = "-90";
        const std::map<float, float> faderMap;

        float map_to_float(const std::map<float, float>& map, float key) {
            auto it = map.lower_bound(key);
            if (it != map.end())
                return it->second;
            if (!map.empty())
                return map.rbegin()->second;
            return 0.0f;
        }
        void set_label(float val);
        
        float get_value() { return value; }
        juce::String get_label() { return label; }
    } Fader;
    ///////////////////////////////////

   public:
    juce::ChangeBroadcaster scribble_broadcaster;
    juce::ChangeBroadcaster state_broadcaster;
    juce::ChangeBroadcaster fader_broadcaster;

   public:
    ChannelStrip() {}
    virtual ~ChannelStrip() {}

    virtual void update_parameter(juce::OSCMessage& message);
    virtual void set_num_and_ap(size_t idx);

    // Setters + Getters
    void set_id(juce::String channel_id) { id = channel_id; }
    juce::String get_id() { return id; }

    juce::OSCAddress get_num_ap() const { return number.ap; }
    size_t get_num_index() const { return number.index; }

    void set_fader_value(float val) { fader.value = val; }
    void set_fader_label(float val) { fader.set_label(val); }
    float get_fader_value() { return fader.get_value(); }
    juce::String get_fader_label() { return fader.get_label(); }

    void set_name(juce::String val) { name.value = val; }
    juce::String get_name() { return name.value; }

    void set_color_value(int val) { color.value = static_cast<COLORS>(val); }
    COLORS get_color_value() { return color.value; }
    juce::String get_color_string() { return color.toString(); }

    void set_state_value(int val) { state.value = static_cast<STATE>(val); }
    juce::String get_state_string() { return state.value ? "on" : "off"; }
    bool get_state() { return state.value; }

   protected:
    void print_string_with_ch_id(juce::OSCMessage& message);

    static juce::String unprocessed_string(juce::OSCMessage& message);
    static juce::String arg_to_str(const juce::OSCArgument& arg);

   protected:
    juce::String id;
    Number number;
    Name name;

    Color color;
    OnState state;
    Fader fader;
};

class DCAChannelStrip : public ChannelStrip {
   public:
    DCAChannelStrip();

    void set_num_and_ap(size_t idx) override;
};

class InputChannelStrip : public ChannelStrip {
   public:
    juce::ChangeBroadcaster dca_broadcaster;

   public:
    InputChannelStrip() {}

    void update_parameter(juce::OSCMessage& message) override;

    // TODO: Fix DCA storing
    void set_dca_assignments(int val);
    std::bitset<8> get_dca_assignments();
    juce::String get_dca_assignments_string();

   protected:
    typedef struct DCA_Assign {
        juce::OSCAddress ap = "/grp/dca";
        // TODO: Make size dynamic
        std::bitset<8> assignments;
    } DCA_Assign;

   private:
    DCA_Assign dca_assignments;
};