#include "class.h"

extern "C" void Object_setVisible(int obj);

extern "C" void KIPlayer_setVisible(KIPlayer *self, bool visible)
{
    int obj = *(int *)((char *)self + 0x8);
    *(uint8_t *)((char *)self + 0xf5) = visible;
    if (obj != 0) {
        return Object_setVisible(obj);
    }
}
