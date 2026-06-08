#include "class.h"

__attribute__((visibility("hidden"))) extern const float g_dm_255_8ee80;

extern "C" void paintcanvas_ext_dm_memcpy(void *dst, const void *src, unsigned int n);
extern "C" void paintcanvas_ext_dm_settrans(void *out, float v);
extern "C" void paintcanvas_ext_dm_getpos(void *out);
extern "C" void paintcanvas_ext_dm_settrans_vec(void *out, void *vec);
extern "C" void paintcanvas_ext_dm_mtx_mul(void *out, const void *a, const void *b);
extern "C" void paintcanvas_ext_dm_mtx_assign(void *dst, const void *src);
extern "C" void paintcanvas_ext_dm_transformvec(void *m, void *vec);
extern "C" void paintcanvas_ext_dm_rotatevec(void *m, void *vec);
extern "C" void paintcanvas_ext_dm_vec_assign(void *dst, const void *src);
extern "C" int paintcanvas_ext_dm_spherefrustum(void *self, void *pt, float radius);
extern "C" float paintcanvas_ext_dm_unsignedtofloat(unsigned int v, unsigned int mode);
extern "C" void paintcanvas_ext_dm_setcolor(void *eng, float r, float g, float b, float a);
extern "C" void paintcanvas_ext_dm_mtx_muleq(void *m, const void *rhs);
extern "C" void paintcanvas_ext_dm_setwvm(void *self, void *m);
extern "C" void paintcanvas_ext_dm_setmodelmatrix(void *eng);
extern "C" void paintcanvas_ext_dm_setuvmatrix(void *eng, void *m);
extern "C" void paintcanvas_ext_dm_meshdraw(void *eng, void *mesh);
extern "C" void paintcanvas_ext_dm_resetuvmatrix(void *eng);
extern "C" void paintcanvas_ext_dm_addcached_mesh(void *mesh, void *arr);
extern "C" void paintcanvas_ext_dm_addcached_uint(unsigned int v, void *arr);
extern "C" void paintcanvas_ext_dm_pushmat(const float *m, void *arr);
extern "C" int paintcanvas_ext_dm_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dm_drawtransform(void *self, void *tf, void *m, void *m2);

// multiply two 8-bit-per-channel packed colors component-wise (>>8)
static unsigned int mulColors(unsigned int a, unsigned int b)
{
    unsigned int r = (((a >> 16) & 0xff) * ((b >> 16) & 0xff)) & 0xffffff00;
    unsigned int g = (((a >> 8) & 0xff) * ((b >> 8) & 0xff)) & 0xffffff00;
    unsigned int bl = ((a & 0xff) * (b & 0xff)) >> 8;
    unsigned int al = (((a >> 24) & 0xff) * ((b >> 24) & 0xff)) & 0xffffff00;
    return (bl) | (g << 8) | (r << 8) | (al << 16);
}

void DrawMesh(AbyssEngine::PaintCanvas *self, char *param_1, const float *param_2,
              const float *param_3, unsigned int param_4, const float *param_5)
{
    char *t = (char *)self;

    float worldM[15];
    float uvM[15];
    paintcanvas_ext_dm_memcpy(worldM, param_2, 0x3c);
    paintcanvas_ext_dm_memcpy(uvM, param_5, 0x3c);

    unsigned int color = param_4;

    // If the mesh has an attached resource (offset 0x34), apply its local transform.
    if (*(int *)(param_1 + 0x34) != 0) {
        float a[16], b[16];
        memset(a, 0, sizeof(a));
        a[0] = 1.0f; a[5] = 1.0f; a[14] = 1.0f;
        memset(b, 0, sizeof(b));
        b[0] = 1.0f; b[5] = 1.0f; b[14] = 1.0f;

        float pivotM[16];
        paintcanvas_ext_dm_settrans(pivotM, *(float *)(param_1 + 0x54));
        paintcanvas_ext_dm_settrans(pivotM, -*(float *)(param_1 + 0x54));
        memset(pivotM, 0, sizeof(pivotM));
        pivotM[0] = 1.0f; pivotM[5] = 1.0f; pivotM[14] = 1.0f;

        float posM[16];
        float resLocal[16];
        paintcanvas_ext_dm_getpos(posM);
        paintcanvas_ext_dm_settrans_vec(resLocal, pivotM);
        paintcanvas_ext_dm_memcpy(resLocal, *(char **)(param_1 + 0x34) + 0x5c, 0x3c);

        float s1[16], s2[16], s3[16];
        paintcanvas_ext_dm_mtx_mul(s1, param_2, pivotM);
        paintcanvas_ext_dm_mtx_mul(s2, s1, b);
        paintcanvas_ext_dm_mtx_mul(s3, s2, resLocal);
        paintcanvas_ext_dm_mtx_mul(posM, s3, a);
        paintcanvas_ext_dm_mtx_assign(worldM, posM);

        unsigned int resColor = *(unsigned int *)(*(char **)(param_1 + 0x34) + 0x48);
        paintcanvas_ext_dm_mtx_assign(uvM, *(char **)(param_1 + 0x34) + 0x98);
        color = mulColors(resColor, param_4);
    }

    float pos[3];
    paintcanvas_ext_dm_memcpy(pos, worldM, 0); // placeholder no-op to anchor worldM use
    paintcanvas_ext_dm_transformvec(uvM, worldM);

    float ext[3];
    ext[0] = *(float *)(param_1 + 0x48);
    ext[1] = ext[0];
    ext[2] = ext[0];
    paintcanvas_ext_dm_rotatevec(uvM, worldM);
    paintcanvas_ext_dm_vec_assign(ext, uvM);

    // radius from the larger absolute component
    float ax = ext[0] < 0.0f ? -ext[0] : ext[0];
    float ay = ext[1] < 0.0f ? -ext[1] : ext[1];
    float az = ext[2] < 0.0f ? -ext[2] : ext[2];
    float maxxy = ax > ay ? ax : ay;
    float radius = maxxy;

    if (*(short *)(param_1 + 2) != 0) {
        float r2 = maxxy > az ? maxxy : az;
        int vis = paintcanvas_ext_dm_spherefrustum(self, uvM, r2 * *(float *)(param_1 + 0x4c));
        if (vis == 0) {
            *(int *)(t + 4) += 1;
            return;
        }
        if (*(short *)(param_1 + 2) != 0) {
            if (*(int *)(param_1 + 0x30) == 0) {
                float fr = paintcanvas_ext_dm_unsignedtofloat(color >> 24, 0);
                float fg = paintcanvas_ext_dm_unsignedtofloat((color >> 16) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat((color >> 8) & 0xff, 0);
                paintcanvas_ext_dm_unsignedtofloat(color & 0xff, 0);
                paintcanvas_ext_dm_setcolor(
                    *(void **)(t + 0x34),
                    (*(float *)(t + 0x1fc) * fr) / g_dm_255_8ee80, 0.0f,
                    (*(float *)(t + 0x200) * fg) / g_dm_255_8ee80, 0.0f);

                paintcanvas_ext_dm_mtx_muleq(worldM, param_3);
                paintcanvas_ext_dm_setwvm(self, worldM);
                paintcanvas_ext_dm_setmodelmatrix(*(void **)(t + 0x34));
                paintcanvas_ext_dm_setuvmatrix(*(void **)(t + 0x34), uvM);
                paintcanvas_ext_dm_meshdraw(*(void **)(t + 0x34), param_1);
                paintcanvas_ext_dm_resetuvmatrix(*(void **)(t + 0x34));
            } else {
                // batch into the glow target at resource+0x44/0x2c/0x38/0x5c/0x50
                char *res = *(char **)(param_1 + 0x30);
                paintcanvas_ext_dm_addcached_mesh(param_1, res + 0x44);
                paintcanvas_ext_dm_pushmat(worldM, res + 0x2c);
                paintcanvas_ext_dm_pushmat(uvM, res + 0x38);
                paintcanvas_ext_dm_pushmat(param_3, res + 0x5c);
                paintcanvas_ext_dm_addcached_uint(color, res + 0x50);
            }
        }
    }

    // recurse over child meshes (count 0x3c / array 0x40) and child transforms (0x4c / 0x50)
    if (*(int *)(param_1 + 0x34) != 0) {
        char *res = *(char **)(param_1 + 0x34);
        unsigned int n = *(unsigned int *)(res + 0x3c);
        while ((int)(--n) >= 0) {
            char *child = (*(char ***)(res + 0x40))[n];
            DrawMesh(self, child, worldM, param_3, param_4, uvM);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(res + 0x4c); i++) {
            if (*(unsigned int *)(t + 0x170) < *(unsigned int *)(t + 0x164)) {
                void *cam = (*(void ***)(t + 0x168))[*(unsigned int *)(t + 0x170)];
                char *tf = (*(char ***)(res + 0x50))[i];
                if (paintcanvas_ext_dm_incamvf(tf, (void *)param_2, cam)) {
                    paintcanvas_ext_dm_drawtransform(self, (*(void ***)(res + 0x50))[i],
                                                     worldM, (void *)param_3);
                }
            }
        }
    }
}
