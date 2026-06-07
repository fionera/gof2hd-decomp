#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightModelfv(unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetGlobalSceneColorAmbient(Engine *self, float red, float green,
                                                        float blue)
{
    *(float *)((char *)self + 0x288) = red;
    *(float *)((char *)self + 0x28c) = green;
    *(float *)((char *)self + 0x290) = blue;
    *(uint32_t *)((char *)self + 0x294) = 0x3f800000;

    if (g_Engine_useShaders == 0) {
        return glLightModelfv(0xb53, (char *)self + 0x288);
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
            (*(float *)(source + 0x268) + red) * *(float *)((char *)self + 0x2a8);
        *(float *)(dest + 0x2d0) =
            (*(float *)(source + 0x26c) + green) * *(float *)((char *)self + 0x2ac);
        *(float *)(dest + 0x2d4) =
            (*(float *)(source + 0x270) + blue) * *(float *)((char *)self + 0x2b0);
    }
    return ShaderUpdateMaterialColor();
}
