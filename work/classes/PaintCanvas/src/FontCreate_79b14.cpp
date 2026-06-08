#include "class.h"

extern "C" void *paintcanvas_ext_fc_findres(void *self, unsigned short id);
extern "C" void paintcanvas_ext_fc_texcreate(void *self, unsigned short id, bool b);
extern "C" int paintcanvas_ext_fc_fontfromfile(void *eng, char *path, unsigned short region, void **out);
extern "C" int paintcanvas_ext_fc_fontheight(void *font);

namespace AbyssEngine {
void ArrayAddImageFont(ImageFont *item, void *array);
}

void FontCreate(AbyssEngine::PaintCanvas *self, unsigned short param_1, unsigned int *param_2,
                bool param_3)
{
    char *res = (char *)paintcanvas_ext_fc_findres(self, param_1);
    if (res == 0) {
        return;
    }
    unsigned short *info = *(unsigned short **)(res + 0xc);
    char *texres = (char *)paintcanvas_ext_fc_findres(self, *info);
    if (texres == 0) {
        return;
    }
    if (*(int *)(texres + 8) == -1) {
        paintcanvas_ext_fc_texcreate(self, *info, true);
    }
    if (*(int *)(res + 8) != -1) {
        *param_2 = *(unsigned int *)(res + 8);
        return;
    }
    void *font = 0;
    char *texpath = (char *)**(char ***)(texres + 0xc);
    int ok = paintcanvas_ext_fc_fontfromfile(*(void **)((char *)self + 0x34), texpath, info[1], &font);
    if (ok != 1) {
        return;
    }
    if (*(int *)(texres + 8) != -1) {
        *(int *)font = *(int *)(texres + 8);
    }
    AbyssEngine::ArrayAddImageFont((AbyssEngine::ImageFont *)font, (char *)self + 0x140);
    int idx = *(int *)((char *)self + 0x140) - 1;
    *(int *)(res + 8) = idx;
    *param_2 = idx;

    char *eng = *(char **)((char *)self + 0x34);
    int cur = *(int *)(eng + 0x78);
    if (cur == -1) {
        *(int *)(eng + 0x78) = idx;
    } else {
        char *curFont = (*(char ***)((char *)self + 0x144))[cur];
        if (*(unsigned short *)curFont <= *(unsigned short *)font) {
            int curH = paintcanvas_ext_fc_fontheight(curFont);
            int newH = paintcanvas_ext_fc_fontheight(font);
            if (newH < curH) {
                *(int *)(*(char **)((char *)self + 0x34) + 0x78) = *param_2;
            }
        }
    }
}
