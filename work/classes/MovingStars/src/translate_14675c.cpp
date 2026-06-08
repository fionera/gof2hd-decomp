#include "class.h"

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

// MovingStars::translate(Vector const&)
//   for each of 50 transforms: pos = MatrixGetPosition(local); pos += v;
//   MatrixSetTranslation(out, local, pos.x, pos.y, pos.z)
extern "C" uint32_t MovingStars_TransformGetLocal(void *canvas, uint32_t tf);     // 0x720c4
extern "C" void MovingStars_MatrixGetPosition(void *out, const void *m);          // 0x6f16c
extern "C" void MovingStars_VectorAddAssign(void *self, const Vector &other);     // 0x73534
extern "C" void MovingStars_MatrixSetTranslation(void *out, const void *base,
                                                 float x, float y, float z);      // 0x6f820

__attribute__((visibility("hidden"))) extern void **g_MovingStars_canvas_translate;

struct MovingStars {
    void translate(const Vector &v);
};

void MovingStars::translate(const Vector &v)
{
    char pos[12];
    char out[60];
    void **canvas = g_MovingStars_canvas_translate;
    for (int i = 0; i != 0x32; i = i + 1) {
        uint32_t tf = F<uint32_t *>(this, 0x4)[i];
        uint32_t loc = MovingStars_TransformGetLocal(*canvas, tf);
        MovingStars_MatrixGetPosition(pos, (const void *)loc);
        MovingStars_VectorAddAssign(pos, v);
        uint32_t tf2 = F<uint32_t *>(this, 0x4)[i];
        uint32_t base = MovingStars_TransformGetLocal(*canvas, tf2);
        MovingStars_MatrixSetTranslation(out, (const void *)base,
                                         *(float *)(pos + 0), *(float *)(pos + 4),
                                         *(float *)(pos + 8));
    }
}
