#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::updateReferenceMatrix() -> (this+0x84) = TransformGetLocal(canvas, tf)
struct AEGeometry { void updateReferenceMatrix(); };
void AEGeometry::updateReferenceMatrix()
{
    Matrix *loc = (Matrix *)PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    Matrix *ref = (Matrix *)((char *)this + 0x84);
    *ref = *loc;
}
