#include "class.h"

__attribute__((visibility("hidden"))) extern float *const g_rpm_fov_8d0dc;
__attribute__((visibility("hidden"))) extern const unsigned int g_rpm_const_8d234;
__attribute__((visibility("hidden"))) extern float *const g_rpm_near_8d196;
__attribute__((visibility("hidden"))) extern float *const g_rpm_far_8d1a4;
__attribute__((visibility("hidden"))) extern char *const g_rpm_flag_8d1b8;

extern "C" int paintcanvas_ext_rpm_dispwidth(void *eng);
extern "C" int paintcanvas_ext_rpm_dispheight(void *eng);
extern "C" float paintcanvas_ext_rpm_sinf(float v);
extern "C" float paintcanvas_ext_rpm_cosf(float v);
extern "C" float paintcanvas_ext_rpm_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_rpm_glMatrixMode(unsigned int mode);
extern "C" void paintcanvas_ext_rpm_glLoadIdentity();
extern "C" void paintcanvas_ext_rpm_glScalef(float x, float y, float z);
extern "C" void paintcanvas_ext_rpm_glLoadMatrixf(void *m);
extern "C" void paintcanvas_ext_rpm_glFinish();
extern "C" void paintcanvas_ext_rpm_loadproj(void *eng, void *m);

void ResetPersMatrix(AbyssEngine::PaintCanvas *self)
{
    char *t = (char *)self;
    int w = paintcanvas_ext_rpm_dispwidth(*(void **)(t + 0x34));
    int h = paintcanvas_ext_rpm_dispheight(*(void **)(t + 0x34));
    float fov = *g_rpm_fov_8d0dc;
    float s = paintcanvas_ext_rpm_sinf(fov * 0.5f);
    float c = paintcanvas_ext_rpm_cosf(fov * 0.5f);
    float fw = paintcanvas_ext_rpm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_rpm_signedtofloat(h, 0);

    memset(t + 0x68, 0, 0x10);
    memset(t + 0x38, 0, 0x10);
    memset(t + 0x58, 0, 0x10);
    memset(t + 0x48, 0, 0x10);

    if (*(unsigned int *)(t + 0x30) <= 3) {
        float aspect = fw / fh;
        float f = 1.0f / (s / c);
        switch (*(unsigned int *)(t + 0x30)) {
        case 0:
            *(float *)(t + 0x48) = f / aspect;
            *(float *)(t + 0x3c) = -f;
            break;
        case 1:
            *(float *)(t + 0x48) = -(f / aspect);
            *(float *)(t + 0x3c) = -f;
            break;
        case 2:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = f / aspect;
            break;
        case 3:
            *(float *)(t + 0x4c) = -f;
            *(float *)(t + 0x38) = -(f / aspect);
            break;
        }
    }

    float n = *g_rpm_near_8d196;
    float fr = *g_rpm_far_8d1a4;
    *(unsigned int *)(t + 0x64) = g_rpm_const_8d234;
    *(float *)(t + 0x60) = (n + fr) / (fr - n);
    *(float *)(t + 0x70) = ((n + n) * fr) / (fr - n);

    if (*g_rpm_flag_8d1b8 == 0) {
        paintcanvas_ext_rpm_glMatrixMode(0x1702);
        paintcanvas_ext_rpm_glLoadIdentity();
        float sc;
        *(unsigned int *)&sc = g_rpm_const_8d234;
        paintcanvas_ext_rpm_glScalef(1.0f, sc, 1.0f);
        paintcanvas_ext_rpm_glMatrixMode(0x1701);
        paintcanvas_ext_rpm_glLoadMatrixf(t + 0x38);
        paintcanvas_ext_rpm_glMatrixMode(0x1700);
        paintcanvas_ext_rpm_glFinish();
    } else {
        paintcanvas_ext_rpm_loadproj(*(void **)(t + 0x34), t + 0x38);
    }
}
