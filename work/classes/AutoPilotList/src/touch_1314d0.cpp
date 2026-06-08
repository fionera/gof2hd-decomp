#include "class.h"

extern "C" void _ZN13AutoPilotList4downEv(AutoPilotList *self);   // AutoPilotList::down()

// AutoPilotList::touch(int p1, int p2) -> selected index, or -1 if the touch is outside
// the list window. On a hit, resets the selection to the top then steps down to the row
// that was touched.
extern "C" int AutoPilotList_touch(AutoPilotList *self, int p1, int p2) {
    int row;
    if (p1 < F<int>(self, 0x4) ||
        F<int>(self, 0x4) + F<int>(self, 0xc) <= p1 ||
        (row = (p2 - F<int>(self, 0x8) - 0xe) / 0xf, p2 - F<int>(self, 0x8) < -0xf) ||
        (uint32_t)(row + 1) >= F<Array<void *> *>(self, 0x10)->length) {
        return -1;
    }
    F<int>(self, 0x0) = 0;
    for (int i = 0; i <= row; i++)
        _ZN13AutoPilotList4downEv(self);
    return F<int>(self, 0x0);
}
