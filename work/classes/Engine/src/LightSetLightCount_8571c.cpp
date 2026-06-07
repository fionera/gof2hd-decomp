#include "class.h"

extern "C" void Engine_LightSetLightCount(Engine *self, int count)
{
    if (count >= 8) {
        count = 8;
    }
    count &= ~(count >> 31);
    *(int *)((char *)self + 0x32c) = count;
}
