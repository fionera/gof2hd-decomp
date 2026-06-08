#include "class.h"

__attribute__((visibility("hidden"))) extern const unsigned int g_tg2di_neg1_8b588;

extern "C" float paintcanvas_ext_tg2di_signedtofloat(int v, unsigned int mode);
extern "C" void paintcanvas_ext_tg2di_meshintersect(void *out, float a, float b, void *mesh);
extern "C" void paintcanvas_ext_tg2di_memcpy(void *dst, void *src, unsigned int n);
extern "C" void paintcanvas_ext_tg2di_invtransformvec(void *outMat, void *vec);
extern "C" void paintcanvas_ext_tg2di_vec_assign(void *dst, void *src);
extern "C" void paintcanvas_ext_tg2di_inner(void *out, void *self, void *childtf, int x, int y);

void TransformGet2DPickedTextureRegion(void *param_1, void *param_2, char *param_3, int param_4,
                                       int param_5, int param_6, unsigned int param_7)
{
    // param_4 = transform ptr (Mesh root container), param_5/6/7 = vec components, shift = param?
    // Per disasm: r5=param_3 (transform), shifts applied with r6 = [r7,#0x10] (shift amount)
    char *tf = param_3;
    int shift = (int)param_7;
    float vy = paintcanvas_ext_tg2di_signedtofloat((param_5 >> shift) >> shift, 0);
    float vx = paintcanvas_ext_tg2di_signedtofloat((param_4 >> shift) >> shift, 0);

    // First: test meshes (offset 0x3c count, 0x40 array)
    unsigned int i = 0;
    bool found = false;
    while (i < *(unsigned int *)(tf + 0x3c)) {
        void *mesh = (*(void ***)(tf + 0x40))[i];
        paintcanvas_ext_tg2di_meshintersect(param_1, vx, vy, mesh);
        i++;
        if (*(float *)param_1 != -1.0f && *(float *)((char *)param_1 + 4) != -1.0f) {
            found = true;
            break;
        }
    }
    if (found) {
        return;
    }

    // Then: recurse over child transforms (0x4c count, 0x50 array)
    float fy = paintcanvas_ext_tg2di_signedtofloat(param_5, 0);
    float fx = paintcanvas_ext_tg2di_signedtofloat(param_4, 0);
    float fz = paintcanvas_ext_tg2di_signedtofloat((int)param_7, 0);

    i = 0;
    while (i < *(unsigned int *)(tf + 0x4c)) {
        char vecbuf[64];
        char matbuf[60];
        char *child = (*(char ***)(tf + 0x50))[i];
        paintcanvas_ext_tg2di_memcpy(vecbuf, child, 0x3c);
        float vin[3];
        vin[0] = fx;
        vin[1] = fy;
        vin[2] = fz;
        paintcanvas_ext_tg2di_invtransformvec(matbuf, vecbuf);
        paintcanvas_ext_tg2di_vec_assign(vin, matbuf);
        paintcanvas_ext_tg2di_inner(param_1, param_2, (*(void ***)(tf + 0x50))[i],
                                    (int)vin[0], (int)vin[1]);
        i++;
        if (*(float *)param_1 != -1.0f && *(float *)((char *)param_1 + 4) != -1.0f) {
            return;
        }
    }
    *(unsigned int *)param_1 = g_tg2di_neg1_8b588;
    *(unsigned int *)((char *)param_1 + 4) = g_tg2di_neg1_8b588;
}
