#include "class.h"

namespace AbyssEngine { namespace AEMath {
struct Matrix { char m[60]; };
} }
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud {
    void render();
};

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" int PaintCanvas_CameraGetCurrent(void *canvas);
extern "C" void *PaintCanvas_CameraGetLocal(void *canvas, int current);
extern "C" void AEGeometry_setScaling(void *geom, float s);
extern "C" void AEGeometry_setDirection(void *geom, Vector const *dir, Vector const *up);
extern "C" void AEGeometry_render(void *geom);

namespace AbyssEngine { namespace AEMath {
void MatrixGetDir(Vector *out, Matrix const *m);
void MatrixGetUp(Vector *out, Matrix const *m);
float operator-(Vector &a, Vector const &b);
} }

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas2;

void PlayerGasCloud::render()
{
    Vector local_80;
    Vector dir;
    char cameraLocal[60];

    if (F<uint8_t>(this, 0xf5) == 0)
        return;
    int mode = F<int>(this, 0x88);
    if (mode != 3 && mode != 0)
        return;

    void *cam = *g_pgc_canvas2;
    int cur = PaintCanvas_CameraGetCurrent(cam);
    __aeabi_memcpy(cameraLocal, PaintCanvas_CameraGetLocal(cam, cur), 0x3c);

    AbyssEngine::AEMath::MatrixGetDir((Vector *)&local_80, (Matrix *)cameraLocal);
    float scale = AbyssEngine::AEMath::operator-(dir, *(Vector *)&local_80);

    void *arr;
    if (F<uint8_t>(this, 0x154) == 0 || (arr = F<void *>(this, 0x138)) == 0) {
        void *geom = F<void *>(this, 0x134);
        AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
        AEGeometry_setDirection(geom, &dir, (Vector *)&local_80);
        AEGeometry_render(geom);
    } else {
        for (unsigned int i = 0, off = 0; i < F<unsigned int>(arr, 0x0); i++, off += 4) {
            void *g = *(void **)((char *)F<void *>(F<void *>(this, 0x138), 0x4) + off);
            float si = *(float *)((char *)F<void *>(F<void *>(this, 0x148), 0x4) + off);
            AEGeometry_setScaling(g, si);
            AbyssEngine::AEMath::MatrixGetUp((Vector *)&local_80, (Matrix *)cameraLocal);
            AEGeometry_setDirection(g, &dir, (Vector *)&local_80);
            AEGeometry_render(g);
            arr = F<void *>(this, 0x138);
        }
    }
}
