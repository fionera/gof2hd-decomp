#include "class.h"

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
void MatrixGetPosition(Matrix *out, const Matrix *m);                 // 0x6f16c
void MatrixSetTranslation(Matrix *m, float x, float y, float z);     // 0x6f820
void Vector_assign(Vector *dst, const Vector *src);                  // 0x6eb3c
}
namespace PaintCanvas {
int  TransformGetTransform(unsigned canvas);     // 0x72088
unsigned CameraGetCurrent();                     // 0x717f4
unsigned CameraGetLocal(unsigned canvas);        // 0x6ff1c
unsigned TransformGetLocal(unsigned canvas);     // 0x720c4
void TransformSetLocal(unsigned canvas, AEMath::Matrix *m);          // 0x721c0
void DrawTransform(unsigned canvas, AEMath::Matrix *m);              // 0x7306c
}
}
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void Sparks_render(Sparks *s);                            // 0x773ec
extern "C" void *__aeabi_memcpy(void *dst, const void *src, unsigned n);

// pc-rel global: holder for the gun-transform canvas pointer (*holder used each iter).
extern unsigned *const gGunRenderCanvas __attribute__((visibility("hidden")));

extern "C" void Gun_render(Gun *self)
{
    Matrix local;
    char camBuf[64];

    Sparks *impact = F<Sparks *>(self, 0xb8);
    if (impact != 0)
        Sparks_render(impact);

    if (F<int>(self, 0x10c) != 0) {
        unsigned canvas = *gGunRenderCanvas;
        for (unsigned i = 0; i < F<unsigned>(self, 0x8); i = i + 1) {
            int tf = AbyssEngine::PaintCanvas::TransformGetTransform(canvas);
            if (*(char *)(tf + 0xed) != 0) {
                unsigned c = canvas;
                AbyssEngine::PaintCanvas::CameraGetCurrent();
                unsigned camLocal = AbyssEngine::PaintCanvas::CameraGetLocal(c);
                __aeabi_memcpy(camBuf, (const void *)camLocal, 0x3c);
                unsigned tl = AbyssEngine::PaintCanvas::TransformGetLocal(canvas);
                AbyssEngine::AEMath::MatrixGetPosition(&local, (const Matrix *)tl);
                AbyssEngine::AEMath::Vector_assign((Vector *)((char *)self + 0xd8), (Vector *)&local);
                AbyssEngine::AEMath::MatrixSetTranslation(&local, F<float>(self, 0xe0), 0, 0);
                Matrix *m = ((Matrix **)F<int>(self, 0x10c))[i];
                AbyssEngine::PaintCanvas::TransformSetLocal(canvas, m);
                AbyssEngine::PaintCanvas::DrawTransform(canvas, m);
            }
        }
    }
}
