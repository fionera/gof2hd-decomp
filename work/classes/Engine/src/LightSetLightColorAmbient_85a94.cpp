#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" void ShaderUpdateMaterialColor();

extern "C" void Engine_LightSetLightColorAmbient(Engine *self, float red, float green,
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
    *(float *)(src + 0x268) = red;
    *(float *)(src + 0x26c) = green;
    *(float *)(src + 0x270) = blue;
    *(uint32_t *)(src + 0x274) = 0x3f800000;
    if (g_Engine_useShaders == 0) {
        return glLightfv(light, 0x1200, src + 0x268);
    }
    char *dst = (char *)self + index * 0x0c;
    *(float *)(dst + 0x2cc) =
        (*(float *)((char *)self + 0x288) + red) * *(float *)((char *)self + 0x2a8);
    *(float *)(dst + 0x2d0) =
        (*(float *)((char *)self + 0x28c) + *(float *)(src + 0x26c)) *
        *(float *)((char *)self + 0x2ac);
    *(float *)(dst + 0x2d4) =
        (*(float *)((char *)self + 0x290) + *(float *)(src + 0x270)) *
        *(float *)((char *)self + 0x2b0);
    return ShaderUpdateMaterialColor();
}
