#include "class.h"

__attribute__((visibility("hidden"))) extern char *const g_dl_flag_794ee;

extern "C" void paintcanvas_ext_dl_glLineWidth(float w);
extern "C" void paintcanvas_ext_dl_glEnable(unsigned int eng, bool on);
extern "C" float paintcanvas_ext_dl_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dl_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dl_glVertexPointer(int a, int b, int c, void *p);
extern "C" void paintcanvas_ext_dl_glColorMask(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dl_glDrawArrays(int a, int b, int c);
extern "C" void paintcanvas_ext_dl_drawline2d(void *eng, int p, bool b);

void DrawLine(AbyssEngine::PaintCanvas *self, int param_1, int param_2, int param_3, int param_4)
{
    char *t = (char *)self;
    char abuf[60];

    float x1 = paintcanvas_ext_dl_signedtofloat(param_1 + 1, 0);
    float y1 = paintcanvas_ext_dl_signedtofloat(param_2, 0);
    float x2 = paintcanvas_ext_dl_signedtofloat(param_3 + 1, 0);
    float y2 = paintcanvas_ext_dl_signedtofloat(param_4, 0);

    float *m = (float *)abuf;
    memset(m, 0, 60);
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[14] = 1.0f;

    float *v = (float *)(t + 0x1d0);

    if (*g_dl_flag_794ee == 0) {
        paintcanvas_ext_dl_glLineWidth(1.0f);
        paintcanvas_ext_dl_glEnable(*(unsigned int *)(t + 0x34), true);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_setwvm(self, abuf);
        paintcanvas_ext_dl_glVertexPointer(2, 0x1406, 0, t + 0x1d0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8074, 1);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8078, 0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8075, 0);
        paintcanvas_ext_dl_glColorMask(*(void **)(t + 0x34), 0x8076, 0);
        paintcanvas_ext_dl_glDrawArrays(1, 0, 2);
        paintcanvas_ext_dl_glEnable(*(unsigned int *)(t + 0x34), true);
    } else {
        paintcanvas_ext_dl_setwvm(self, abuf);
        v[0] = x1;
        v[1] = y1;
        v[2] = x2;
        v[3] = y2;
        paintcanvas_ext_dl_drawline2d(*(void **)(t + 0x34), (int)(t + 0x1d0), true);
    }
}
