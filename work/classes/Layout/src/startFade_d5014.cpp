#include "class.h"

// Layout::startFade(bool, int, int)
extern "C" void Layout_startFade(Layout *self, uint8_t fadeOut, int color, int duration) {
    F<uint8_t>(self, 0x401) = fadeOut;
    F<uint8_t>(self, 0x400) = 1;
    F<uint32_t>(self, 0x404) = color & 0xffffff00u;
    F<int>(self, 0x408) = 0;
    F<int>(self, 0x40c) = duration;
}
