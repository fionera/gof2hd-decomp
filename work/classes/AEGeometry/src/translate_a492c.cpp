#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::translate(float, float, float)
extern "C" void *__aeabi_memcpy(void *d, const void *s, uint32_t n);
extern "C" void _ae_MatrixSetTranslation(void *out, float x, float y, float z);
extern "C" void _ae_TransformSetLocal(uint32_t canvas, uint32_t tf, void *m);

struct AEGeometry { void translate(float x, float y, float z); };
void AEGeometry::translate(float x, float y, float z)
{
    char buf[60];
    char src[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    __aeabi_memcpy(src, (void *)loc, 0x3c);
    _ae_MatrixSetTranslation(buf, *(float *)(src + 0x0c) + z, *(float *)(src + 0x1c),
                             *(float *)(src + 0x2c) + y);
    _ae_TransformSetLocal(this->f_2c, this->f_c, buf);
    (void)x;
}
