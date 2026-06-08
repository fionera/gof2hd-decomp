#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getParentPosition() -> tf = this->0x24; if (tf==-1) tf = this->0xc; then matrix pos
struct AEGeometry { Vector getParentPosition(); };
Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = this->f_2c;
    uint32_t tf = this->f_24;
    if (tf + 1 == 0)
        tf = this->f_c;
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(canvas, tf);
    return MatrixGetPosition(loc);
}
