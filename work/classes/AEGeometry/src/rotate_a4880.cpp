#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::rotate(float, float, float)
extern "C" void _ae_MatrixSetRotation(void *out, uint32_t loc, float x, float y, float z, int order);
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);

struct AEGeometry { void rotate(float x, float y, float z); };
void AEGeometry::rotate(float x, float y, float z)
{
    char buf[60];
    f32(this, 0x30) = f32(this, 0x30) + x;
    f32(this, 0x34) = f32(this, 0x34) + y;
    f32(this, 0x38) = f32(this, 0x38) + z;
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetRotation(buf, loc, f32(this, 0x30), f32(this, 0x34), f32(this, 0x38),
                          i32(this, 0x4c));
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling(buf, loc, f32(this, 0x3c), f32(this, 0x40), f32(this, 0x44));
}
