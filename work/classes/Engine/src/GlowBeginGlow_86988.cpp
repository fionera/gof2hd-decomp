#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glColorMask(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
extern "C" void Engine_GlowEnableGlow(Engine *self);
extern "C" void glDepthFunc(unsigned int func);

extern "C" void Engine_GlowBeginGlow(Engine *self, unsigned int depthFunc)
{
    if (*(uint8_t *)((char *)self + 0x41c) == 0 && g_Engine_useShaders != 0) {
        glColorMask(0, 0, 0, 1);
        Engine_GlowEnableGlow(self);
        if (*(uint8_t *)((char *)self + 0x41c) != 0) {
            return glDepthFunc(depthFunc);
        }
    }
}
