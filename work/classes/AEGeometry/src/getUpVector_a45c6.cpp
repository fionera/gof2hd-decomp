#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getUpVector() -> TransformGetLocal(canvas,tf); return MatrixGetUp(loc)
struct AEGeometry { Vector getUpVector(); };
Vector AEGeometry::getUpVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    return MatrixGetUp(loc);
}
