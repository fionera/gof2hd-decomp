#include "class.h"

extern "C" void glEnableClientState(unsigned int array);
extern "C" void glDisableClientState(unsigned int array);

extern "C" void Engine_AEClientState(Engine *self, unsigned int state, bool enable)
{
    uint32_t bits = *(uint32_t *)((char *)self + 0x4a4);
    uint32_t mask = 0;
    switch (state) {
    case 0x8074: mask = 2; break;
    case 0x8075: mask = 1; break;
    case 0x8076: mask = 8; break;
    case 0x8078: mask = 4; break;
    default: return;
    }
    if (enable) {
        if ((bits & mask) != 0) {
            return;
        }
        glEnableClientState(state);
        bits |= mask;
    } else {
        if ((bits & mask) == 0) {
            return;
        }
        glDisableClientState(state);
        bits &= ~mask;
    }
    *(uint32_t *)((char *)self + 0x4a4) = bits;
}
