#include "class.h"

__attribute__((visibility("hidden"))) extern const double g_cipvf_gravscale_7bba8;

extern "C" void paintcanvas_ext_cipvf_matidentity(void *out, void *m);
extern "C" void *paintcanvas_ext_cipvf_getgrav(void *eng);
extern "C" float paintcanvas_ext_cipvf_sinf(float v);
extern "C" float paintcanvas_ext_cipvf_cosf(float v);
extern "C" int paintcanvas_ext_cipvf_inner(void *pt, void *m, void *cam);

int CameraIsPointinViewFrustum(AbyssEngine::PaintCanvas *self, void *param_1)
{
    char *t = (char *)self;
    if (*(unsigned int *)(t + 0x170) >= *(unsigned int *)(t + 0x164)) {
        return 1;
    }

    // identity matrix scratch
    float m[16];
    memset(m, 0, sizeof(m));
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[14] = 1.0f;
    char scratch[60];
    paintcanvas_ext_cipvf_matidentity(scratch, m);

    void *grav = paintcanvas_ext_cipvf_getgrav(*(void **)(t + 0x34));
    double angle = *(double *)((char *)grav + 8) * g_cipvf_gravscale_7bba8;
    float a = (float)angle;
    int orient = *(int *)(t + 0x30);
    float rot = (orient == 1) ? a : -a;
    float s = paintcanvas_ext_cipvf_sinf(rot);
    float c = paintcanvas_ext_cipvf_cosf(rot);
    m[0] = c;
    m[5] = c;
    *(unsigned int *)&m[1] = *(unsigned int *)&s ^ 0x80000000;
    m[4] = s;

    void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
    return paintcanvas_ext_cipvf_inner(param_1, m, cam);
}
