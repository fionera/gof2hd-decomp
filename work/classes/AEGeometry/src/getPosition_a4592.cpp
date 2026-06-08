#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getPosition() -> TransformGetLocal(canvas,tf); return MatrixGetPosition(loc)
struct AEGeometry { Vector getPosition(); };
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetPosition(loc);
}
