#include "class.h"

extern "C" char *paintcanvas_ext_rt_getAEChar(void *strField);
extern "C" int paintcanvas_ext_rt_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" void paintcanvas_ext_rt_deletearr(char *p);

void ReloadTextures(AbyssEngine::PaintCanvas *self)
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x10); i++) {
        char *res = (*(char ***)((char *)self + 0x14))[i];
        if (*(int *)res == -1) {
            char *path = paintcanvas_ext_rt_getAEChar(res + 4);
            float f = *(float *)((*(char ***)((char *)self + 0x14))[i] + 0x10);
            int ok = paintcanvas_ext_rt_texfromfile(*(void **)((char *)self + 0x34), path, 0, 0,
                                                    &out, false, f);
            if (ok == 1) {
                *(int *)((*(char ***)((char *)self + 0x14))[i]) = 0;
            }
            paintcanvas_ext_rt_deletearr(path);
        }
    }
}
