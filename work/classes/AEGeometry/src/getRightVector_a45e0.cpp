#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getRightVector() -> TransformGetLocal(canvas,tf); return MatrixGetRight(loc)
struct AEGeometry { Vector getRightVector(); };
Vector AEGeometry::getRightVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetRight(loc);
}
