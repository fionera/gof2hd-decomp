#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setScaling(float, float, float)
extern "C" void _ae_MatrixSetRotation(void *out, uint32_t loc, float x, float y, float z, int order);
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);

struct AEGeometry { void setScaling(float x, float y, float z); };
void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    _ae_MatrixSetRotation(buf, loc, this->f_30, this->f_34, this->f_38,
                          this->f_4c);
    loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    this->f_3c = x;
    this->f_40 = y;
    this->f_44 = z;
}
