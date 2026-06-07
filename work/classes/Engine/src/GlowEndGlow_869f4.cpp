#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glColorMask(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
extern "C" void glDepthFunc(unsigned int func);

extern "C" void Engine_GlowEndGlow(Engine *self)
{
    if (g_Engine_useShaders == 0) {
        return;
    }
    *(uint8_t *)((char *)self + 0x41c) = 0;
    glColorMask(1, 1, 1, 1);
    return glDepthFunc(0x201);
}
