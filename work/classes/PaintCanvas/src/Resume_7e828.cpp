#include "class.h"

__attribute__((visibility("hidden"))) extern int *const g_resume_curtex_7e828;

extern "C" char *paintcanvas_ext_rs_getAEChar(void *strField);
extern "C" int paintcanvas_ext_rs_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" void paintcanvas_ext_rs_deletearr(char *p);
extern "C" void paintcanvas_ext_rs_glActiveTexture(unsigned int tex);
extern "C" void paintcanvas_ext_rs_glBindTexture(unsigned int target, unsigned int tex);

void Resume(AbyssEngine::PaintCanvas *self)
{
    unsigned int out = 0;
    for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x10); i++) {
        char *res = (*(char ***)((char *)self + 0x14))[i];
        char *path = paintcanvas_ext_rs_getAEChar(res + 4);
        float f = *(float *)((*(char ***)((char *)self + 0x14))[i] + 0x10);
        int ok = paintcanvas_ext_rs_texfromfile(*(void **)((char *)self + 0x34), path, 0, 0,
                                                &out, false, f);
        if (ok == 1) {
            *(int *)((*(char ***)((char *)self + 0x14))[i]) = 0;
        }
        paintcanvas_ext_rs_deletearr(path);
    }
    int *cur = g_resume_curtex_7e828;
    if (*cur != 0) {
        paintcanvas_ext_rs_glActiveTexture(0x84c7);
        char *res = (*(char ***)((char *)self + 0x14))[*cur];
        paintcanvas_ext_rs_glBindTexture(0x8513, *(unsigned int *)res);
        paintcanvas_ext_rs_glActiveTexture(0x84c0);
    }
}
