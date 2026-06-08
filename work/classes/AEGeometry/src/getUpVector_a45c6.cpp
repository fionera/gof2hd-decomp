#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getUpVector() -> TransformGetLocal(canvas,tf); return MatrixGetUp(loc)
struct AEGeometry { Vector getUpVector(); };
Vector AEGeometry::getUpVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetUp(loc);
}
