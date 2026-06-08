#include "class.h"

extern "C" void *paintcanvas_ext_tc_findres(void *self, unsigned short id);
extern "C" int paintcanvas_ext_tc_texfromfile(void *eng, char *path, void *cb, void *ud,
                                              unsigned int *out, bool b, float f);
extern "C" int paintcanvas_ext_tc_texfromfileintern(void *eng, char *path, void *cb, void *ud,
                                                    unsigned int *out, float f, void *lt, bool b);

void TextureCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, void *param_2,
                   void *param_3, unsigned int *param_4, bool param_5)
{
    void *eng = *(void **)((char *)self + 0x34);
    *(unsigned int *)((char *)eng + 0x7c) = 0xffffffff;
    *(unsigned int *)((char *)eng + 0x80) = 0xffffffff;

    char *res = (char *)paintcanvas_ext_tc_findres(self, param_1);
    if (res != 0) {
        unsigned int idx = *(unsigned int *)(res + 8);
        if (idx == 0xffffffff) {
            void **info = *(void ***)(res + 0xc);
            float f = (float)(int)(*(unsigned int *)((char *)info + 4));
            char *path = (char *)*info;
            int ok;
            if (!param_5) {
                ok = paintcanvas_ext_tc_texfromfileintern(*(void **)((char *)self + 0x34), path,
                                                          param_2, param_3, &idx, f, 0, false);
            } else {
                ok = paintcanvas_ext_tc_texfromfile(*(void **)((char *)self + 0x34), path,
                                                    param_2, param_3, &idx, true, f);
            }
            if (ok != 1) {
                return;
            }
            idx = 0;
            *(unsigned int *)(res + 8) = 0;
        }
        *param_4 = idx;
    }
}
