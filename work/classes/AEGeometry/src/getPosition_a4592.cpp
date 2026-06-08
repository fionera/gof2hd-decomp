#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getPosition() -> TransformGetLocal(canvas,tf); return MatrixGetPosition(loc)
struct AEGeometry { Vector getPosition(); };
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    return MatrixGetPosition(loc);
}
