#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodChildTransform(unsigned int)
struct AEGeometry { void setLodChildTransform(uint32_t param); };
void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = i32(this, 0x50);
    if (count > 0) {
        uint32_t *arr = new uint32_t[count];
        pp(this, 0x58) = arr;
        for (int i = 0; i < count; i++) {
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], param);
            count = i32(this, 0x50);
        }
    }
}
