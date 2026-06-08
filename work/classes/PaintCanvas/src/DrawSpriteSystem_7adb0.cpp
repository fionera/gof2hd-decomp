#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_dss2_gravscale_8af58;

extern "C" void paintcanvas_ext_dss2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss2_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss2_sinf(float v);
extern "C" float paintcanvas_ext_dss2_cosf(float v);
extern "C" float paintcanvas_ext_dss2_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_dss2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss2_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss2_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss2_ssdraw(void *eng, void *worldM, void *viewM, void *ss);

void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1,
                      const float *matA, const float *matB)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    if ((*(void ***)(t + 0x184))[param_1] == 0) {
        return;
    }

    // local copies: matA -> world (local_a0), matB -> view (local_64)
    float world[15];
    float view[15];
    memcpy(world, matA, sizeof(world));
    memcpy(view, matB, sizeof(view));

    if (*t == 0) {
        float inv[16];
        memset(inv, 0, sizeof(inv));
        inv[0] = 1.0f;
        inv[5] = 1.0f;
        inv[14] = 1.0f;
        paintcanvas_ext_dss2_mtx_getinv(inv, view);
        paintcanvas_ext_dss2_mtx_assign(view, inv);
    } else {
        float rotM[16];
        char scratch[60];
        memset(rotM, 0, sizeof(rotM));
        rotM[0] = 1.0f;
        rotM[5] = 1.0f;
        rotM[14] = 1.0f;
        paintcanvas_ext_dss2_matidentity(scratch, rotM);

        void *grav = paintcanvas_ext_dss2_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_dss2_gravscale_8af58;
        int ia = (int)(long long)angle;
        if (*(int *)(t + 0x30) == 1) {
            ia = -ia;
        }
        float a = paintcanvas_ext_dss2_signedtofloat(ia, 0);
        float s = paintcanvas_ext_dss2_sinf(a);
        float c = paintcanvas_ext_dss2_cosf(a);
        rotM[0] = c;
        rotM[5] = c;
        *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
        rotM[4] = s;

        paintcanvas_ext_dss2_mtx_muleq(view, rotM);
        paintcanvas_ext_dss2_mtx_getinv(scratch, view);
        paintcanvas_ext_dss2_mtx_assign(view, scratch);
        paintcanvas_ext_dss2_mtx_muleq(world, rotM);
    }

    paintcanvas_ext_dss2_ssdraw(*(void **)(t + 0x34), world, view,
                                (*(void ***)(t + 0x184))[param_1]);
}
