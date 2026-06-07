#include "class.h"

extern "C" void paintcanvas_ext_string_prep(AbyssEngine::PaintCanvas *, void *, int);
extern "C" unsigned int paintcanvas_ext_strlen(void *);
extern "C" void paintcanvas_ext_drawstring_str(void *, unsigned int, unsigned int, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void DrawString(AbyssEngine::PaintCanvas *self, unsigned int index, void *str,
                int x, int y, bool b)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        char *font = (*(char ***)((char *)self + 0x144))[index];
        paintcanvas_ext_string_prep(self, *(void **)(font + 0x8), -1);
        char *font2 = (*(char ***)((char *)self + 0x144))[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        paintcanvas_ext_drawstring_str(font2, len, *(unsigned int *)((char *)str + 0x8), x, y,
                                       self, *(void **)((char *)self + 0x34), b);
    }
}
