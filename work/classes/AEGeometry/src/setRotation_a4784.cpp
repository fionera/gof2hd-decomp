#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setRotation(float, float, float)
extern "C" void _ae_MatrixSetRotation(void *out, uint32_t loc, float x, float y, float z, int order);
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);

struct AEGeometry { void setRotation(float x, float y, float z); };
void AEGeometry::setRotation(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    _ae_MatrixSetRotation(buf, loc, x, y, z, this->f_4c);
    loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    _ae_MatrixSetScaling(buf, loc, this->f_3c, this->f_40, this->f_44);
    this->f_30 = x;
    this->f_34 = y;
    this->f_38 = z;
}
