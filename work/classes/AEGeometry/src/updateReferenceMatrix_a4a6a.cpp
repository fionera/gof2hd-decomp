#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::updateReferenceMatrix() -> (this+0x84) = TransformGetLocal(canvas, tf)
struct AEGeometry { void updateReferenceMatrix(); };
void AEGeometry::updateReferenceMatrix()
{
    Matrix *loc = (Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    Matrix *ref = (Matrix *)((char *)this + 0x84);
    *ref = *loc;
}
