#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetLightColorSpecular(Engine *self, float red, float green,
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
    *(float *)(src + 0x248) = red;
    *(float *)(src + 0x24c) = green;
    *(float *)(src + 0x250) = blue;
    *(uint32_t *)(src + 0x254) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1202, src + 0x248);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2e4) = *(float *)((char *)self + 0x2b8) * red;
    *(float *)(dst + 0x2e8) = *(float *)(src + 0x24c) * *(float *)((char *)self + 0x2bc);
    *(float *)(dst + 0x2ec) = *(float *)(src + 0x250) * *(float *)((char *)self + 0x2c0);
    return ShaderUpdateMaterialColor();
}
