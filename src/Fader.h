#pragma once

#include "faderValueMap.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_osc/juce_osc.h>

class Fader : public juce::Component
{
private:
    juce::OSCAddress ap{juce::String("/ch/01/mix/fader")};
    float value;
    // juce::Label printout;

    float map_to_float(std::map<float, float> map, float key)
    {
        using namespace std;

        float out;
        try
        {
            out = map.at(key);
        }
        catch (const std::exception &)
        {
            auto it = map.lower_bound(key);
            if (it != map.end())
                out = it->second;
            else if (!map.empty())
                out = map.rbegin()->second; // fallback to largest key's value
            else
                out = 0.0f;
        }

        return out;
    }

public:
    Fader()
    {
        value = -90;
    }
    void set_value(float val) { value = map_to_float(faderMap, val); }
    float get_value() { return value; }

    // void paint() override {}
};