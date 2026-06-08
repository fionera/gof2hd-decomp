#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getMatrix() -> return *(Matrix*)TransformGetLocal(canvas, tf)  (returns a reference)
struct AEGeometry { Matrix &getMatrix(); };
Matrix &AEGeometry::getMatrix()
{
    return *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
}
