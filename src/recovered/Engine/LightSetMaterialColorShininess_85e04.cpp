#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMaterialf(unsigned int face, unsigned int pname, float value);

extern "C" void Engine_LightSetMaterialColorShininess(Engine *self, float shininess)
{
    *(float *)((char *)self + 0x2c8) = shininess;
    if (g_Engine_useShaders == 0) {
        return glMaterialf(0x408, 0x1601, shininess);
    }
}
