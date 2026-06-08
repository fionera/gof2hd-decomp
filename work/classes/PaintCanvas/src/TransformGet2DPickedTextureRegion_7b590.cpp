#include "class.h"

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2d_defval_7b590;

extern "C" void paintcanvas_ext_tg2d_memcpy(void *dst, void *src, unsigned int n);
extern "C" float paintcanvas_ext_tg2d_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_tg2d_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_tg2d_vec_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tg2d_inner(void *out, void *self, void *tf, int x, int y);
extern "C" void paintcanvas_ext_tg2d_errmsg(void *out);

void TransformGet2DPickedTextureRegion(void *param_1, char *param_2, int param_3, int param_4,
                                       int param_5, int param_6)
{
    char matbuf[60];
    char vecbuf[64];

    if ((unsigned int)param_3 < *(unsigned int *)(param_2 + 0x158)) {
        char *tf = (*(char ***)(param_2 + 0x15c))[param_3];
        paintcanvas_ext_tg2d_memcpy(vecbuf, tf, 0x3c);
        float local74 = paintcanvas_ext_tg2d_signedtofloat(param_5, 0);
        float local70 = paintcanvas_ext_tg2d_signedtofloat(param_6, 0);
        float local6c = paintcanvas_ext_tg2d_signedtofloat(param_4, 0);
        float vin[3];
        vin[0] = local74;
        vin[1] = local70;
        vin[2] = local6c;
        paintcanvas_ext_tg2d_invtransformvec(matbuf, vecbuf);
        paintcanvas_ext_tg2d_vec_assign(vin, matbuf);
        paintcanvas_ext_tg2d_inner(param_1, param_2, tf, (int)vin[0], (int)vin[1]);
    } else {
        *(unsigned int *)param_1 = g_tg2d_defval_7b590;
        *(unsigned int *)((char *)param_1 + 4) = g_tg2d_defval_7b590;
        paintcanvas_ext_tg2d_errmsg(param_1);
    }
}
