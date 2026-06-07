#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" void *paintcanvas_ext_getscreenpos_m(void *self, void *m, Vector *a, Vector *b);

void *GetScreenPosition(AbyssEngine::PaintCanvas *self, Vector *a, Vector *b)
{
    char buf[60];
    float *m = (float *)buf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;
    return paintcanvas_ext_getscreenpos_m(self, buf, a, b);
}
