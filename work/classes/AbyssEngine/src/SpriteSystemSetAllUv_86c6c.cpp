#include "class.h"

// AbyssEngine::SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem*)
// Fill the UV quad (4 corners, interleaved u/v) for every sprite. The shipped code flips V
// (1.0 - v) for the bottom edge, matching a top-left texture origin.
namespace AbyssEngine {

void SpriteSystemSetAllUv(float u0, float v0, float u1, float v1, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    unsigned short count = u16(sys, 0x0);
    char *uvBase = (char *)pp(pp(sys, 0x8), 0x8);

    for (unsigned int idx = 0; (idx & 0xffff) < (unsigned int)(count << 3); idx += 8) {
        float *p = (float *)(uvBase + (idx & 0xffff) * 4);
        p[0] = u0;
        p[1] = 1.0f - v0;
        p[2] = u1;
        p[3] = 1.0f - v0;
        p[4] = u1;
        p[5] = 1.0f - v1;
        p[6] = u0;
        p[7] = 1.0f - v1;
    }
}

} // namespace AbyssEngine
