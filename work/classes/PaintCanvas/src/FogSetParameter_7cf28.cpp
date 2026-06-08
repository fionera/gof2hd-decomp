#include "class.h"

__attribute__((visibility("hidden"))) extern char *const g_fsp_flag_8cf40;
__attribute__((visibility("hidden"))) extern const double g_fsp_255d_8d070;
__attribute__((visibility("hidden"))) extern const float g_fsp_255f_8d078;

extern "C" float paintcanvas_ext_fsp_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_fsp_glFogf(unsigned int pname, float v);
extern "C" void paintcanvas_ext_fsp_glFogfv(unsigned int pname, void *v);
extern "C" void paintcanvas_ext_fsp_vec_assign(void *dst, void *src);

void FogSetParameter(AbyssEngine::PaintCanvas *self, int param_1, float param_2, float param_3,
                     float param_4, unsigned int param_5)
{
    char *t = (char *)self;
    float col[4];
    if (*g_fsp_flag_8cf40 == 0) {
        float density = paintcanvas_ext_fsp_unsignedtofloat((unsigned int)param_1, 0);
        paintcanvas_ext_fsp_glFogf(0xb65, density);
        paintcanvas_ext_fsp_glFogf(0xb62, param_4);
        paintcanvas_ext_fsp_glFogf(0xb63, param_2);
        paintcanvas_ext_fsp_glFogf(0xb64, param_3);

        float r = paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 16) & 0xff, 0);
        float g = paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 8) & 0xff, 0);
        paintcanvas_ext_fsp_unsignedtofloat(param_5 >> 24, 0);
        float b = paintcanvas_ext_fsp_unsignedtofloat(param_5 & 0xff, 0);
        col[0] = r / g_fsp_255f_8d078;
        col[1] = g / g_fsp_255f_8d078;
        col[2] = b / g_fsp_255f_8d078;
        col[3] = param_2;
        paintcanvas_ext_fsp_glFogfv(0xb66, col);
    } else {
        double r = (double)paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 16) & 0xff, 0);
        r = r / g_fsp_255d_8d070;
        paintcanvas_ext_fsp_unsignedtofloat(param_5 >> 24, 0);
        double g = (double)paintcanvas_ext_fsp_unsignedtofloat((param_5 >> 8) & 0xff, 0);
        char *eng = *(char **)(t + 0x34);
        g = g / g_fsp_255d_8d070;
        *(float *)(eng + 1000) = param_2;
        *(float *)(eng + 0x3ec) = param_3;
        col[0] = (float)r;
        col[1] = (float)g;
        paintcanvas_ext_fsp_vec_assign(eng + 0x3f0, col);
    }
}
