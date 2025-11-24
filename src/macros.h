#pragma once

#include <juce_core/juce_core.h>

#define DBG_STR " (" << __FILE_NAME__ << " : " << __LINE__ << ")"

// static void MAKE_BUSY() {
//     int c = 0;
//     while (c < 500)
//     {
//         DBG("mt " << c++);
//         juce::Thread::sleep(100);
//     }
// }