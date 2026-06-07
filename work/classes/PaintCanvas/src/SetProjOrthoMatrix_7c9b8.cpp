#include "class.h"

#define HIDDEN __attribute__((visibility("hidden")))
extern float *paintcanvas_g_pom_persp HIDDEN;
extern float *paintcanvas_g_pom_a HIDDEN;
extern float *paintcanvas_g_pom_b HIDDEN;
extern "C" void paintcanvas_ext_setprojmatrix3d(void *, float, float, float);
extern "C" int paintcanvas_ext_getdisplaywidth(void *);
extern "C" int paintcanvas_ext_getdisplayheight(void *);

void SetProjOrthoMatrix(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    float g = *paintcanvas_g_pom_persp;
    if (g != -1.0f) {
        paintcanvas_ext_setprojmatrix3d(self, g, *paintcanvas_g_pom_a, *paintcanvas_g_pom_b);
    }
    void *eng = *(void **)(t + 0x34);
    float *r;
    r = (float *)(t + 0xa8); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x98); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x88); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;
    r = (float *)(t + 0x78); r[0] = 0.0f; r[1] = 0.0f; r[2] = 0.0f; r[3] = 0.0f;

    int w = paintcanvas_ext_getdisplaywidth(eng);
    *(float *)(t + 0x78) = (float)(2.0 / (double)w);

    int h = paintcanvas_ext_getdisplayheight(*(void **)(t + 0x34));
    *(float *)(t + 0xa0) = -0.05f;
    *(float *)(t + 0xb4) = 1.0f;
    *(float *)(t + 0xa8) = -1.0f;
    *(float *)(t + 0xac) = 1.0f;
    *(float *)(t + 0x8c) = -(float)(2.0 / (double)h);
}
