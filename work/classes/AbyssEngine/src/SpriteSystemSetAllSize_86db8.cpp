#include "class.h"

// AbyssEngine::SpriteSystemSetAllSize(short, AbyssEngine::SpriteSystem*)
// Write a single size value to every sprite (or just the first when the "shared size"
// flag at +0xc is set).
namespace AbyssEngine {

void SpriteSystemSetAllSize(short size, SpriteSystem *sys)
{
    if (sys == 0)
        return;

    short *sizes = (short *)pp(sys, 0x8);
    if (u8(sys, 0xc) != 0) {
        sizes[0] = size;
        return;
    }
    unsigned int count = u16(sys, 0x0);
    for (unsigned int i = 0; i < count; ++i)
        sizes[i] = size;
}

} // namespace AbyssEngine
