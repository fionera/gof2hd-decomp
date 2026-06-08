#include "class.h"

__attribute__((visibility("hidden"))) extern float *const g_spm_p0_7b826;
__attribute__((visibility("hidden"))) extern float *const g_spm_p1_7b82a;
__attribute__((visibility("hidden"))) extern float *const g_spm_p2_7b82c;
__attribute__((visibility("hidden"))) extern float *const g_spm_p3_7b82e;
__attribute__((visibility("hidden"))) extern float *const g_spm_p4_7b830;
__attribute__((visibility("hidden"))) extern const unsigned int g_spm_const_7b950;

extern "C" int paintcanvas_ext_spm_dispwidth(void *eng);
extern "C" int paintcanvas_ext_spm_dispheight(void *eng);
extern "C" float paintcanvas_ext_spm_sinf(float v);
extern "C" float paintcanvas_ext_spm_cosf(float v);
extern "C" float paintcanvas_ext_spm_signedtofloat(int v, unsigned int mode);

void SetProjectionMatrix3d(AbyssEngine::PaintCanvas *self, float param_1, float param_2,
                           float param_3)
{
    char *t = (char *)self;

    // stash parameters into the engine globals
    *g_spm_p0_7b826 = param_3;
    *g_spm_p1_7b82a = param_2;
    *g_spm_p4_7b830 = param_1;
    *g_spm_p2_7b82c = param_2;
    *g_spm_p3_7b82e = param_3;

    int w = paintcanvas_ext_spm_dispwidth(*(void **)(t + 0x34));
    int h = paintcanvas_ext_spm_dispheight(*(void **)(t + 0x34));
    float half = param_1 * 0.5f;
    float s = paintcanvas_ext_spm_sinf(half);
    float c = paintcanvas_ext_spm_cosf(half);
    float fw = paintcanvas_ext_spm_signedtofloat(w, 0);
    float fh = paintcanvas_ext_spm_signedtofloat(h, 0);

    // zero the four matrix rows at 0x38,0x48,0x58,0x68
    memset(t + 0x68, 0, 0x10);
    memset(t + 0x58, 0, 0x10);
    memset(t + 0x48, 0, 0x10);
    memset(t + 0x38, 0, 0x10);

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

    *(unsigned int *)(t + 0x64) = g_spm_const_7b950;
    *(float *)(t + 0x60) = (param_2 + param_3) / (param_2 - param_3);
    *(float *)(t + 0x70) = ((param_3 + param_3) * param_2) / (param_2 - param_3);
}
