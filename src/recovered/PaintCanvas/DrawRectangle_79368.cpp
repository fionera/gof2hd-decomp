#include "class.h"

__attribute__((visibility("hidden"))) extern char *const g_dr_flag_79368;

extern "C" void paintcanvas_ext_dr_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dr_glLineWidth(float w);
extern "C" void paintcanvas_ext_dr_glcap(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dr_glVertexPointer(int a, int b, int c, void *p);
extern "C" void paintcanvas_ext_dr_glColorMask(void *eng, unsigned int cap, int on);
extern "C" void paintcanvas_ext_dr_glDrawArrays(int a, int b, int c);
extern "C" void paintcanvas_ext_dr_drawline2d(void *eng, int p, int n, bool b);

void DrawRectangle(AbyssEngine::PaintCanvas *self, int param_1, int param_2,
                   int param_3, int param_4)
{
    char abuf[60];

    double dw = (double)param_3;
    double dh = (double)param_4;
    double dy = (double)param_2;
    double dx = (double)param_1;
    float fVar4 = (float)(dw - 0.5 + dx);
    float fVar5 = (float)(dh - 0.5 + dy);

    float *v = (float *)((char *)self + 0x1d0);
    v[0] = (float)(dx + 0.5);
    v[1] = (float)(dy + 0.5);
    v[2] = fVar4;
    v[3] = (float)(dy + 0.5);
    v[4] = fVar4;
    v[5] = fVar5;
    v[6] = (float)(dx + 0.5);
    v[7] = fVar5;

    float *m = (float *)abuf;
    m[0] = 1.0f;
    m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
    m[10] = 1.0f; m[11] = 0.0f; m[12] = 1.0f; m[13] = 1.0f;
    m[14] = 1.0f;

    paintcanvas_ext_dr_setwvm(self, abuf);
    if (*g_dr_flag_79368 == 0) {
        paintcanvas_ext_dr_glLineWidth(1.0f);
        paintcanvas_ext_dr_glcap(*(void **)((char *)self + 0x34), 0xde1, 0);
        paintcanvas_ext_dr_glVertexPointer(2, 0x1406, 0, (char *)self + 0x1d0);
        paintcanvas_ext_dr_glColorMask(*(void **)((char *)self + 0x34), 0x8074, 1);
        paintcanvas_ext_dr_glColorMask(*(void **)((char *)self + 0x34), 0x8078, 0);
        paintcanvas_ext_dr_glColorMask(*(void **)((char *)self + 0x34), 0x8075, 0);
        paintcanvas_ext_dr_glColorMask(*(void **)((char *)self + 0x34), 0x8076, 0);
        paintcanvas_ext_dr_glDrawArrays(2, 0, 4);
        paintcanvas_ext_dr_glcap(*(void **)((char *)self + 0x34), 0xde1, 1);
    } else {
        paintcanvas_ext_dr_drawline2d(*(void **)((char *)self + 0x34), (int)((char *)self + 0x1d0), 4, true);
    }
}
