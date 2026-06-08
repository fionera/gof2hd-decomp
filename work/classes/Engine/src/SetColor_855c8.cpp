#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void Engine_LightSetMaterialColorAlpha(Engine *self, float alpha);
extern "C" void ShaderUpdateMaterialColor();
extern "C" void glColor4f(float red, float green, float blue, float alpha);

extern "C" void Engine_SetColor(Engine *self, float red, float green, float blue, float alpha)
{
    if (*(float *)((char *)self + 0xd0) == red &&
        *(float *)((char *)self + 0xd4) == green &&
        *(float *)((char *)self + 0xd8) == blue &&
        *(float *)((char *)self + 0xdc) == alpha) {
        return;
    }
    *(float *)((char *)self + 0xd4) = green;
    *(float *)((char *)self + 0xd0) = red;
    *(float *)((char *)self + 0xd8) = blue;
    *(float *)((char *)self + 0xdc) = alpha;
    *(int *)((char *)self + 0xe0) =
        (int)(green * 255.0f) * 0x10000 + (int)(red * 255.0f) * 0x1000000 +
        (int)(blue * 255.0f) * 0x100 + (int)(alpha * 255.0f);
    if (g_Engine_useShaders != 0) {
        return ShaderUpdateMaterialColor();
    }
    Engine_LightSetMaterialColorAlpha(self, alpha);
    return glColor4f(red, green, blue, alpha);
}
