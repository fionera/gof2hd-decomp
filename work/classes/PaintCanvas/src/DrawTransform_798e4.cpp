#include "class.h"

extern "C" void paintcanvas_ext_mtx_mul(void *out, void *a, void *b);
extern "C" void paintcanvas_ext_mtx_muleq(void *m, void *rhs);
extern "C" void paintcanvas_ext_dt_drawmesh(void *self, void *mesh, void *m, void *m2,
                                            unsigned int flag, void *m3);
extern "C" int paintcanvas_ext_dt_incamvf(void *tf, void *m, void *cam);
extern "C" void paintcanvas_ext_dt_drawtransform_rec(void *self, void *tf, void *m, void *m3);

void DrawTransform(AbyssEngine::PaintCanvas *self, char *tf, void *m2, void *m3)
{
    char buf[60];
    if (tf && *(unsigned char *)(tf + 0xec)) {
        paintcanvas_ext_mtx_mul(buf, m2, tf);
        if (*(int *)(tf + 0x11c) != 0) {
            paintcanvas_ext_mtx_muleq(buf, tf + 0x5c);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x3c); i++) {
            void *mesh = (*(void ***)(tf + 0x40))[i];
            paintcanvas_ext_dt_drawmesh(self, mesh, buf, m3, *(unsigned int *)(tf + 0x48), tf + 0x98);
        }
        for (unsigned int i = 0; i < *(unsigned int *)(tf + 0x4c); i++) {
            if (*(unsigned int *)((char *)self + 0x170) < *(unsigned int *)((char *)self + 0x164) &&
                paintcanvas_ext_dt_incamvf((*(void ***)(tf + 0x50))[i], buf,
                    (*(void ***)((char *)self + 0x168))[*(unsigned int *)((char *)self + 0x170)])) {
                paintcanvas_ext_dt_drawtransform_rec(self, (*(void ***)(tf + 0x50))[i], buf, m3);
            } else {
                *(int *)((char *)self + 0x4) += 1;
            }
        }
    }
}
