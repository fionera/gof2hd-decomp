#include "class.h"

typedef void Materialfv(unsigned int face, unsigned int pname, const void *params);
static Materialfv * volatile g_Engine_glMaterialfv;

extern "C" void Engine_LightSetMaterialColorAlpha(Engine *self, float alpha)
{
    if (*(uint8_t *)((char *)self + 0x424) == 0) {
        return;
    }

    *(float *)((char *)self + 0x2b4) = alpha;
    *(float *)((char *)self + 0x488) = alpha;
    Materialfv *materialfv = g_Engine_glMaterialfv;
    materialfv(0x408, 0x1200, (char *)self + 0x2a8);
    *(uint32_t *)((char *)self + 0x2c4) = *(uint32_t *)((char *)self + 0x488);
    materialfv(0x408, 0x1202, (char *)self + 0x2b8);
    *(uint32_t *)((char *)self + 0x2a4) = *(uint32_t *)((char *)self + 0x488);
    return materialfv(0x408, 0x1201, (char *)self + 0x298);
}
