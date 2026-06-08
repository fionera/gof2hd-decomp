#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_dss1_gravscale_8ac10;

extern "C" void paintcanvas_ext_dss1_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dss1_getgrav(void *eng);
extern "C" float paintcanvas_ext_dss1_sinf(float v);
extern "C" float paintcanvas_ext_dss1_cosf(float v);
extern "C" void paintcanvas_ext_dss1_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_dss1_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dss1_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dss1_mtx_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_dss1_ssdraw(void *eng, void *ident, void *m, void *ss);

void DrawSpriteSystem(AbyssEngine::PaintCanvas *self, unsigned int param_1)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x180)) {
        return;
    }
    if ((*(void ***)(t + 0x184))[param_1] == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f;
    worldM[5] = 1.0f;
    worldM[14] = 1.0f;

    if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
        if (*t == 0) {
            float inv[16];
            memset(inv, 0, sizeof(inv));
            inv[0] = 1.0f;
            inv[5] = 1.0f;
            inv[14] = 1.0f;
            paintcanvas_ext_dss1_mtx_getinv(inv, worldM);
            paintcanvas_ext_dss1_mtx_assign(worldM, inv);
        } else {
            float rotM[16];
            char scratch[60];
            memset(rotM, 0, sizeof(rotM));
            rotM[0] = 1.0f;
            rotM[5] = 1.0f;
            rotM[14] = 1.0f;
            paintcanvas_ext_dss1_matidentity(scratch, rotM);

            void *grav = paintcanvas_ext_dss1_getgrav(*(void **)(t + 0x34));
            double angle = *(double *)((char *)grav + 8) * g_dss1_gravscale_8ac10;
            float a = (float)angle;
            int orient = *(int *)(t + 0x30);
            float rot = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dss1_sinf(rot);
            float c = paintcanvas_ext_dss1_cosf(rot);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            // copy the active camera's view matrix (at cam+0xc) into scratch
            char *cam = (*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
            paintcanvas_ext_dss1_memcpy(scratch, cam + 0xc, 0x3c);
            paintcanvas_ext_dss1_mtx_muleq(scratch, rotM);
            paintcanvas_ext_dss1_mtx_getinv(scratch, scratch);
            paintcanvas_ext_dss1_mtx_assign(worldM, scratch);
        }
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f;
    ident[5] = 1.0f;
    ident[14] = 1.0f;
    paintcanvas_ext_dss1_ssdraw(*(void **)(t + 0x34), ident, worldM,
                                (*(void ***)(t + 0x184))[param_1]);
}
