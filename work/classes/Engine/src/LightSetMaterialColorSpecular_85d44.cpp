#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetMaterialColorSpecular(Engine *self, float red, float green,
                                                      float blue)
{
    *(float *)((char *)self + 0x2b8) = red;
    *(float *)((char *)self + 0x2bc) = green;
    *(float *)((char *)self + 0x2c0) = blue;
    *(uint32_t *)((char *)self + 0x2c4) = *(uint32_t *)((char *)self + 0x488);

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1202, (char *)self + 0x2b8);
    }

    int count = *(int *)((char *)self + 0x32c);
    int sourceOffset = 0;
    int destOffset = 0;
    for (int index = 0; index < count; index += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2e4) = *(float *)(source + 0x248) * red;
        *(float *)(dest + 0x2e8) = *(float *)(source + 0x24c) * green;
        *(float *)(dest + 0x2ec) = *(float *)(source + 0x250) * blue;
    }
    return ShaderUpdateMaterialColor();
}
