#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void ShaderUpdateRimColor();

extern "C" void Engine_LightSetRimColor(Engine *self, float red, float green, float blue)
{
    if (g_Engine_useShaders == 0) {
        return;
    }
    *(float *)((char *)self + 0x320) = red;
    *(float *)((char *)self + 0x324) = green;
    *(float *)((char *)self + 0x328) = blue;
    return ShaderUpdateRimColor();
}
