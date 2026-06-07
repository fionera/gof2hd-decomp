#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetMaterialColorAmbient(Engine *self, float red, float green,
                                                     float blue)
{
    *(float *)((char *)self + 0x2a8) = red;
    *(float *)((char *)self + 0x2ac) = green;
    *(float *)((char *)self + 0x2b0) = blue;
    *(uint32_t *)((char *)self + 0x2b4) = *(uint32_t *)((char *)self + 0x488);

    if (g_Engine_useShaders == 0) {
        return glMaterialfv(0x408, 0x1200, (char *)self + 0x2a8);
    }

    int count = *(int *)((char *)self + 0x32c);
    int sourceOffset = 0;
    int destOffset = 0;
    for (int index = 0; index < count; index += 1) {
        char *source = (char *)self + sourceOffset;
        char *dest = (char *)self + destOffset;
        sourceOffset += 0x10;
        destOffset += 0x0c;
        *(float *)(dest + 0x2cc) =
            (*(float *)((char *)self + 0x288) + *(float *)(source + 0x268)) * red;
        *(float *)(dest + 0x2d0) =
            (*(float *)((char *)self + 0x28c) + *(float *)(source + 0x26c)) * green;
        *(float *)(dest + 0x2d4) =
            (*(float *)((char *)self + 0x290) + *(float *)(source + 0x270)) * blue;
    }
    return ShaderUpdateMaterialColor();
}
