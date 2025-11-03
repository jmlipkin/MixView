#pragma once

#include "faderValueMap.h"

#include <bitset>
#include <map>

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class ChannelStrip
{
    private:
protected:
    typedef enum Color
    {
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
    };

    typedef std::bitset<8> DCA_assign;
    typedef juce::String Name;

public:
    typedef struct Fader {
        float value;

        float map_to_float(std::map<float, float> map, float key)
        {
            using namespace std;

            float out;
            try
            {
                out = map.at(key);
            }
            catch (const exception &)
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
        void set_value(float val) { value = map_to_float(faderMap, val); }
        float get_value() { return value; }
    };

    ChannelStrip() {}
};

