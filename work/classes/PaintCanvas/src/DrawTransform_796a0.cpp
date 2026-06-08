#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_dt_gravscale_898d8;

extern "C" void paintcanvas_ext_dt2_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_dt2_getgrav(void *eng);
extern "C" float paintcanvas_ext_dt2_sinf(float v);
extern "C" float paintcanvas_ext_dt2_cosf(float v);
extern "C" int paintcanvas_ext_dt2_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dt2_mtx_assign(void *dst, const void *src);
extern "C" void paintcanvas_ext_dt2_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dt2_mtx_getinv(void *out, void *m);
extern "C" void paintcanvas_ext_dt2_seteye(void *eng, float a, float b, float c);
extern "C" void paintcanvas_ext_dt2_drawrec(void *self, void *tf, void *m, void *m2);

void DrawTransform(AbyssEngine::PaintCanvas *self, unsigned int param_1, const float *param_2)
{
    char *t = (char *)self;
    if (param_1 >= *(unsigned int *)(t + 0x158)) {
        return;
    }
    char *tf = (*(char ***)(t + 0x15c))[param_1];
    if (*(unsigned char *)(tf + 0xec) == 0) {
        return;
    }

    float worldM[16];
    memset(worldM, 0, sizeof(worldM));
    worldM[0] = 1.0f; worldM[5] = 1.0f; worldM[14] = 1.0f;

    if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
        void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
        if (*t == 0) {
            int vis = paintcanvas_ext_dt2_incamvf((*(void ***)(t + 0x15c))[param_1], 0, cam);
            if (vis == 0) {
                *(int *)(t + 4) += 1;
                return;
            }
            float inv[16];
            paintcanvas_ext_dt2_mtx_getinv(inv, worldM);
            paintcanvas_ext_dt2_mtx_assign(worldM, inv);
        } else {
            float rotM[16];
            char scratch[60];
            memset(rotM, 0, sizeof(rotM));
            rotM[0] = 1.0f; rotM[5] = 1.0f; rotM[14] = 1.0f;
            paintcanvas_ext_dt2_matidentity(scratch, rotM);

            void *grav = paintcanvas_ext_dt2_getgrav(*(void **)(t + 0x34));
            double angle = *(double *)((char *)grav + 8) * g_dt_gravscale_898d8;
            float a = (float)angle;
            int orient = *(int *)(t + 0x30);
            float ang = (orient == 1) ? a : -a;
            float s = paintcanvas_ext_dt2_sinf(ang);
            float c = paintcanvas_ext_dt2_cosf(ang);
            rotM[0] = c;
            rotM[5] = c;
            *(unsigned int *)&rotM[1] = *(unsigned int *)&s ^ 0x80000000;
            rotM[4] = s;

            int vis = paintcanvas_ext_dt2_incamvf((*(void ***)(t + 0x15c))[param_1], rotM, cam);
            if (vis == 0) {
                *(int *)(t + 4) += 1;
                return;
            }

            float viewM[16];
            const float *src = param_2;
            if (src == 0) {
                src = (const float *)((*(char ***)(t + 0x168))[*(unsigned int *)(t + 0x170)] + 0xc);
            }
            paintcanvas_ext_dt2_mtx_assign(viewM, src);
            paintcanvas_ext_dt2_mtx_muleq(viewM, rotM);
            paintcanvas_ext_dt2_mtx_getinv(scratch, viewM);
            paintcanvas_ext_dt2_mtx_assign(worldM, scratch);
        }
        void *cam2 = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
        paintcanvas_ext_dt2_seteye(*(void **)(t + 0x34),
                                   *(float *)((char *)cam2 + 0x18),
                                   *(float *)((char *)cam2 + 0x28),
                                   *(float *)((char *)cam2 + 0x38));
        tf = (*(char ***)(t + 0x15c))[param_1];
    }

    float ident[16];
    memset(ident, 0, sizeof(ident));
    ident[0] = 1.0f; ident[5] = 1.0f; ident[14] = 1.0f;
    paintcanvas_ext_dt2_drawrec(self, tf, ident, worldM);
}
