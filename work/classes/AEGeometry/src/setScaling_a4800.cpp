#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setScaling(float, float, float)
extern "C" void _ae_MatrixSetRotation(void *out, uint32_t loc, float x, float y, float z, int order);
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);

struct AEGeometry { void setScaling(float x, float y, float z); };
void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetRotation(buf, loc, f32(this, 0x30), f32(this, 0x34), f32(this, 0x38),
                          i32(this, 0x4c));
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    f32(this, 0x3c) = x;
    f32(this, 0x40) = y;
    f32(this, 0x44) = z;
}
