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
    *(uint32_t *)((char *)self + 0xe0) =
        ((uint32_t)(green * 255.0f) << 16) + ((uint32_t)(red * 255.0f) << 24) +
        ((uint32_t)(blue * 255.0f) << 8) + (uint32_t)(alpha * 255.0f);
    if (g_Engine_useShaders != 0) {
        return ShaderUpdateMaterialColor();
    }
    Engine_LightSetMaterialColorAlpha(self, alpha);
    return glColor4f(red, green, blue, alpha);
}
