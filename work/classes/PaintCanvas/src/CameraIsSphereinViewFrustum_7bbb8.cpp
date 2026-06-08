#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_cisvf_gravscale_7bcd8;

extern "C" void paintcanvas_ext_cisvf_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_cisvf_getgrav(void *eng);
extern "C" float paintcanvas_ext_cisvf_sinf(float v);
extern "C" float paintcanvas_ext_cisvf_cosf(float v);
extern "C" int paintcanvas_ext_cisvf_inner(void *pt, float radius, void *m, void *cam);

int CameraIsSphereinViewFrustum(AbyssEngine::PaintCanvas *self, void *param_1, float param_2)
{
    char *t = (char *)self;
    if (param_2 == 0.0f ||
        *(unsigned int *)(t + 0x164) <= *(unsigned int *)(t + 0x170)) {
        return 1;
    }

    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cisvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cisvf_getgrav(*(void **)(t + 0x34));
    double angle = *(double *)((char *)grav + 8) * g_cisvf_gravscale_7bcd8;
    float a = (float)angle;
    int orient = *(int *)(t + 0x30);
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cisvf_sinf(rot);
    float c = paintcanvas_ext_cisvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    return paintcanvas_ext_cisvf_inner(param_1, param_2, m, cam);
}
