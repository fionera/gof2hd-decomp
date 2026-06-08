#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getMatrix() -> return *(Matrix*)TransformGetLocal(canvas, tf)  (returns a reference)
struct AEGeometry { Matrix &getMatrix(); };
Matrix &AEGeometry::getMatrix()
{
    return *(Matrix *)PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
}
