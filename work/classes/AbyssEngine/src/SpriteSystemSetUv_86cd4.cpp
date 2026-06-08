#include "class.h"

// AbyssEngine::SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d,
//                                AbyssEngine::SpriteSystem*)
// Set the interleaved UV quad (8 floats) of one sprite. A global flag selects whether the
// "v" lanes are flipped (1.0 - v). Slot layout mirrors the shipped routine.
namespace AbyssEngine {

extern "C" char *g_SpriteSystem_uvFlipFlag; // *(DAT_00096d40 + 0x96cfe)

void SpriteSystemSetUv(unsigned short idx, float a, float b, float c, float d, SpriteSystem *sys)
{
    unsigned int count = 0;
    if (sys != 0)
        count = u16(sys, 0x0);
    if (sys == 0 || idx >= count)
        return;

    char flag = *g_SpriteSystem_uvFlipFlag;
    float *p = (float *)((char *)pp(pp(sys, 0x8), 0x8) + (unsigned int)idx * 0x20);

    // In the binary, `c` (in_r2) and `d` (in_stack_00000000) are flipped when flag==0.
    if (flag == 0) {
        c = 1.0f - c;
        d = 1.0f - d;
    }
    p[0] = a;
    p[1] = c;
    p[2] = b;
    p[3] = c;
    p[4] = b;
    p[5] = d;
    p[6] = a;
    p[7] = d;
}

} // namespace AbyssEngine
