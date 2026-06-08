#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetMaterialColorDiffuse(Engine *self, float red, float green, float blue)
{
    *(float *)((char *)self + 0x298) = red;
    *(float *)((char *)self + 0x29c) = green;
    *(float *)((char *)self + 0x2a0) = blue;
    *(uint32_t *)((char *)self + 0x2a4) = *(uint32_t *)((char *)self + 0x488);

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1201, (char *)self + 0x298);
    }

    int lightCount = *(int *)((char *)self + 0x32c);
    int sourceOffset = 0;
    int destOffset = 0;
    for (int i = 0; i < lightCount; i += 1) {
        int source = (int)(uint32_t)(char *)self + sourceOffset;
        int dest = (int)(uint32_t)(char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2fc) = *(float *)(source + 0x228) * red;
        *(float *)(dest + 0x300) = *(float *)(source + 0x22c) * green;
        *(float *)(dest + 0x304) = *(float *)(source + 0x230) * blue;
    }
    return ShaderUpdateMaterialColor();
}
