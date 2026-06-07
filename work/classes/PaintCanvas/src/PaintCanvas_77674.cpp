#include "class.h"

// Array/Matrix constructors (distinct callees grouped by template instantiation).
extern "C" void paintcanvas_ctor_arr_tex(void *);    // 0x6f88c  f10
extern "C" void paintcanvas_ctor_arr_mesh(void *);   // 0x6f724  f24, f18c
extern "C" void paintcanvas_ctor_matrix(void *);     // 0x6f118  f8
extern "C" void paintcanvas_ctor_arr_res(void *);    // 0x6f898  f134
extern "C" void paintcanvas_ctor_arr_font(void *);   // 0x6f8a4  f140
extern "C" void paintcanvas_ctor_arr_img(void *);    // 0x6f8b0  f14c
extern "C" void paintcanvas_ctor_arr_tf(void *);     // 0x6f730  f158
extern "C" void paintcanvas_ctor_arr_cam(void *);    // 0x6f8bc  f164
extern "C" void paintcanvas_ctor_arr_mat(void *);    // 0x6f8c8  f174
extern "C" void paintcanvas_ctor_arr_ss(void *);     // 0x6f8d4  f180
extern "C" void paintcanvas_ctor_arr_mtx(void *);    // 0x6f8e0  f198, f1a4, f1bc
extern "C" void paintcanvas_ctor_arr_uint(void *);   // 0x6f8ec  f1b0
extern "C" void paintcanvas_ext_meshcreate5(void *, unsigned short, unsigned short,
                                            signed char, void *);

AbyssEngine::PaintCanvas *PaintCanvasCtor(AbyssEngine::PaintCanvas *self, AbyssEngine::Engine *engine)
{
    char *t = (char *)self;
    paintcanvas_ctor_arr_tex(t + 0x10);
    paintcanvas_ctor_arr_mesh(t + 0x24);
    paintcanvas_ctor_matrix(t + 0xf8);
    paintcanvas_ctor_arr_res(t + 0x134);
    paintcanvas_ctor_arr_font(t + 0x140);
    paintcanvas_ctor_arr_img(t + 0x14c);
    paintcanvas_ctor_arr_tf(t + 0x158);
    paintcanvas_ctor_arr_cam(t + 0x164);
    paintcanvas_ctor_arr_mat(t + 0x174);
    paintcanvas_ctor_arr_ss(t + 0x180);
    paintcanvas_ctor_arr_mesh(t + 0x18c);
    paintcanvas_ctor_arr_mtx(t + 0x198);
    paintcanvas_ctor_arr_mtx(t + 0x1a4);
    paintcanvas_ctor_arr_uint(t + 0x1b0);
    paintcanvas_ctor_arr_mtx(t + 0x1bc);

    *(unsigned char *)(t + 0x1f1) = 0;
    *(unsigned int *)(t + 0x20) = 0;
    *(unsigned int *)(t + 0x1f4) = 1;
    *(unsigned int *)(t + 0x4) = 0;
    *(unsigned char *)(t + 0x0) = 0;
    *(AbyssEngine::Engine **)(t + 0x34) = engine;
    *(unsigned int *)(t + 0x170) = 0xffffffff;

    paintcanvas_ext_meshcreate5(engine, 4, 2, 0x11, t + 0x1c8);

    int *p = *(int **)(*(char **)(t + 0x1c8) + 0x2c);
    p[0] = 0x20000;
    p[1] = 1;
    p[2] = *(int *)0x87878;
    *(float *)(t + 0x1fc) = 1.0f;
    *(float *)(t + 0x200) = 1.0f;
    *(float *)(t + 0x204) = 1.0f;
    *(float *)(t + 0x208) = 1.0f;
    *(unsigned char *)(t + 0x1c) = 1;
    *(unsigned char *)((char *)engine + 0xfc) = 1;

    paintcanvas_ext_meshcreate5(engine, 400, 200, 0x1b, t + 0x8);

    short *buf = *(short **)(*(char **)(t + 0x8) + 0x2c);
    int j = 0;
    for (int i = 0; i != 0x4b0; i += 0xc) {
        short *e = buf + i / 2;
        short a = (short)(j << 2);
        e[0] = a;
        e[1] = (short)((j << 2) | 2);
        e[2] = (short)((j << 2) | 1);
        e[3] = a;
        e[4] = (short)((j << 2) | 3);
        e[5] = (short)((j << 2) | 2);
        j++;
    }
    *(unsigned char *)(t + 0x1f8) = 1;
    *(unsigned int *)(t + 0x1cc) = 0;
    return self;
}
