#include "class.h"

// Begins a maneuver gesture: mark active, record start position, reset timer.
extern "C" void MGame_maneuverTouchBegin(MGame *self, int x, void *p) {
    F<uint8_t>(self, 0x17c) = 1;
    F<int>(self, 0x180) = x;
    F<void *>(self, 0x184) = p;
    F<int>(self, 0x178) = 0;
}
