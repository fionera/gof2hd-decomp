#include "class.h"

extern "C" void ChoiceWindow_update(void *cw, int dt);  // blx 0x74eb4

// Layout::update(int dt)
extern "C" void Layout_update(Layout *self, int dt) {
    void *cw = F<void *>(self, 0x3c4);
    if (cw)
        ChoiceWindow_update(cw, dt);
    if (F<uint8_t>(self, 0x2ec) != 0) {
        int t = F<int>(self, 0x3d0) + dt;
        F<int>(self, 0x3d0) = t;
        if (t >= 6999 + 1)        // 0x1b58 = 7000; cmp r0,#7000; mov.ge
            F<uint8_t>(self, 0x2ec) = 0;
    }
    if (F<uint8_t>(self, 0x400) != 0) {
        int t = F<int>(self, 0x408) + dt;
        int dur = F<int>(self, 0x40c);
        F<int>(self, 0x408) = t;
        if (t > dur) {
            F<uint8_t>(self, 0x400) = 0;
            F<int>(self, 0x408) = 0;
        }
    }
}
