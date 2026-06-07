#include "class.h"

extern "C" void glClearColor(float red, float green, float blue, float alpha);
extern "C" void glClear(unsigned int mask);

extern "C" void Engine_GlowEnableGlow(Engine *self)
{
    if (*(uint8_t *)((char *)self + 0x41c) != 0) {
        return;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(0x4000);
    *(uint8_t *)((char *)self + 0x41c) = 1;
}
