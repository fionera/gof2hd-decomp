#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_dss_gravscale_8ada0;

extern "C" void paintcanvas_ext_dss_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss_sinf(float v);
extern "C" float paintcanvas_ext_dss_cosf(float v);
extern "C" void paintcanvas_ext_dss_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss_ssdraw(void *eng, void *ident, void *m, void *ss);

void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1, const float *mat)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    void *ss = (*(void ***)(t + 0x184))[param_1];
    if (ss == 0) {
        return;
    }

    // local copy of the incoming matrix
    float local[15];
    memcpy(local, mat, sizeof(local));

    float identbuf[16];
    char scratch[60];
    float inv[16];

    if (*t == 0) {
        paintcanvas_ext_dss_mtx_getinv(inv, local);
        paintcanvas_ext_dss_mtx_assign(local, inv);
    } else {
        memset(identbuf, 0, sizeof(identbuf));
        identbuf[0] = 1.0f;
        identbuf[5] = 1.0f;
        identbuf[14] = 1.0f;
        paintcanvas_ext_dss_matidentity(scratch, identbuf);

        void *grav = paintcanvas_ext_dss_getgrav(*(void **)(t + 0x34));
        double angle = *(double *)((char *)grav + 8) * g_dss_gravscale_8ada0;
        float a = (float)angle;
        int orient = *(int *)(t + 0x30);
        float rot = (orient == 1) ? a : -a;
        float s = paintcanvas_ext_dss_sinf(rot);
        float c = paintcanvas_ext_dss_cosf(rot);
        identbuf[0] = c;
        identbuf[5] = c;
        *(unsigned int *)&identbuf[1] = *(unsigned int *)&s ^ 0x80000000;
        identbuf[4] = s;

        paintcanvas_ext_dss_mtx_muleq(local, identbuf);
        paintcanvas_ext_dss_mtx_getinv(scratch, local);
        paintcanvas_ext_dss_mtx_assign(local, scratch);
    }

    float ident2[16];
    memset(ident2, 0, sizeof(ident2));
    ident2[0] = 1.0f;
    ident2[5] = 1.0f;
    ident2[14] = 1.0f;
    paintcanvas_ext_dss_ssdraw(*(void **)(t + 0x34), ident2, local,
                               (*(void ***)(t + 0x184))[param_1]);
}
