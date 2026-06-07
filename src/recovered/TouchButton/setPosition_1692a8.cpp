#include "class.h"

extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags)
{
    unsigned int f = (unsigned int)flags;
    I(self, 0x78) = x;
    I(self, 0x7c) = y;
    UC(self, 0x74) = flags;
    if ((flags & 0x20) != 0) {
        y = y - I(self, 0x8c);
        I(self, 0x7c) = y;
    }
    if ((int)(f << 0x1e) < 0) {
        x = x - I(self, 0x90);
        I(self, 0x78) = x;
    }
    if ((int)(f << 0x19) < 0) {
        I(self, 0x7c) = y - I(self, 0x8c) / 2;
    }
    if ((int)(f << 0x1d) < 0) {
        I(self, 0x78) = x - I(self, 0x90) / 2;
    }
}
