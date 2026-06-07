#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetLightColorDiffuse(Engine *self, float red, float green,
                                                  float blue, unsigned int light)
{
    unsigned int index = light - 0x4000;
    if (index > 7) {
        return;
    }
    int count = light - 0x3fff;
    int current = *(int *)((char *)self + 0x32c);
    if (current > count) {
        count = current;
    }
    *(int *)((char *)self + 0x32c) = count;

    char *src = (char *)self + index * 0x10;
    *(float *)(src + 0x228) = red;
    *(float *)(src + 0x22c) = green;
    *(float *)(src + 0x230) = blue;
    *(uint32_t *)(src + 0x234) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1201, src + 0x228);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2fc) = *(float *)((char *)self + 0x298) * red;
    *(float *)(dst + 0x300) = *(float *)(src + 0x22c) * *(float *)((char *)self + 0x29c);
    *(float *)(dst + 0x304) = *(float *)(src + 0x230) * *(float *)((char *)self + 0x2a0);
    return ShaderUpdateMaterialColor();
}
