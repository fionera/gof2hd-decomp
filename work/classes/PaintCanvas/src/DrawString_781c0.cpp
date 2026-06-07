#include "class.h"

extern "C" void paintcanvas_ext_string_prep(AbyssEngine::PaintCanvas *, void *, int);
extern "C" void paintcanvas_ext_drawstring_raw(void *, const unsigned short *, int, int,
                                               AbyssEngine::PaintCanvas *, void *, bool);

void DrawString(AbyssEngine::PaintCanvas *self, unsigned int index, const unsigned short *str,
                int x, int y, bool b)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        char *font = (*(char ***)((char *)self + 0x144))[index];
        paintcanvas_ext_string_prep(self, *(void **)(font + 0x8), -1);
        char *font2 = (*(char ***)((char *)self + 0x144))[index];
        paintcanvas_ext_drawstring_raw(font2, str, x, y, self,
                                       *(void **)((char *)self + 0x34), b);
    }
}
