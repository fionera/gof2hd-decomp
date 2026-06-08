#include "class.h"

// AbyssEngine::SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a,
//                                  AbyssEngine::SpriteSystem*)
// Write the same RGBA color to all four vertices (16 floats) of one sprite.
namespace AbyssEngine {

void SpriteSystemSetRGBA(unsigned short idx, float r, float g, float b, float a, SpriteSystem *sys)
{
    unsigned int count = 0;
    if (sys != 0)
        count = u16(sys, 0x0);
    if (sys == 0 || idx >= count)
        return;

    float *p = (float *)((char *)pp(pp(sys, 0x8), 0xc) + (unsigned int)idx * 0x40);
    for (int v = 0; v < 4; ++v) {
        p[v * 4 + 0] = r;
        p[v * 4 + 1] = g;
        p[v * 4 + 2] = b;
        p[v * 4 + 3] = a;
    }
}

} // namespace AbyssEngine
