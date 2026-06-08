#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getRightVector() -> TransformGetLocal(canvas,tf); return MatrixGetRight(loc)
struct AEGeometry { Vector getRightVector(); };
Vector AEGeometry::getRightVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    return MatrixGetRight(loc);
}
