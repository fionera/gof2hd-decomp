#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getParentPosition() -> tf = this->0x24; if (tf==-1) tf = this->0xc; then matrix pos
struct AEGeometry { Vector getParentPosition(); };
Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = u32(this, 0x2c);
    uint32_t tf = u32(this, 0x24);
    if (tf + 1 == 0)
        tf = u32(this, 0xc);
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(canvas, tf);
    return MatrixGetPosition(loc);
}
