#include "class.h"

namespace AbyssEngine {
void ArrayAddMesh(Mesh *item, void *array);
}

extern "C" void paintcanvas_ext_tami_bsphere_merge(void *dst, void *src);
extern "C" void paintcanvas_ext_tami_setanimlen(void *tf, int hi, int lo);
extern "C" void paintcanvas_ext_tami_setanimstate(void *tf, int a, int b);
extern "C" void paintcanvas_ext_tami_finalize(void *tf);

void TransformAddMeshId(AbyssEngine::PaintCanvas *self, unsigned int param_1, unsigned int param_2)
{
    if (param_1 >= *(unsigned int *)((char *)self + 0x158) ||
        param_2 >= *(unsigned int *)((char *)self + 0x24)) {
        return;
    }
    char *tf = (*(char ***)((char *)self + 0x15c))[param_1];
    char *mesh = (*(char ***)((char *)self + 0x28))[param_2];
    AbyssEngine::ArrayAddMesh(*(AbyssEngine::Mesh **)(mesh), tf + 0x3c);
    paintcanvas_ext_tami_bsphere_merge(tf + 0xd4, mesh + 0x3c);

    void *res = *(void **)(mesh + 0x34);
    if (res != 0) {
        long long resLen = *(long long *)((char *)res + 0xf8);
        long long tfLen = *(long long *)(tf + 0xf8);
        if (tfLen < resLen) {
            paintcanvas_ext_tami_setanimlen(tf, (int)(resLen >> 32), (int)resLen);
            tf = (*(char ***)((char *)self + 0x15c))[param_1];
        }
        long long tfStart = *(long long *)(tf + 0x100);
        void *res2 = *(void **)(mesh + 0x34);
        long long resStart = *(long long *)((char *)res2 + 0x100);
        if (tfStart == 0 || resStart < tfStart) {
            *(long long *)(tf + 0x100) = resStart;
        }
        paintcanvas_ext_tami_setanimstate(tf, 2, 0);
    }
    paintcanvas_ext_tami_finalize((*(void ***)((char *)self + 0x15c))[param_1]);
}
