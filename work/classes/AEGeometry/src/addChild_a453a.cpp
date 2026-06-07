#include "class.h"

using namespace AbyssEngine;

// AEGeometry::addChild(unsigned int child)
struct AEGeometry { void addChild(uint32_t child); };
void AEGeometry::addChild(uint32_t child)
{
    PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c), u32(this, 0xc), child);
    uint32_t old = u32(this, 0x14);
    if (old != 0xffffffffu)
        u32(this, 0x10) = old;
    u32(this, 0x14) = child;
}
