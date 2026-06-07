#include "class.h"

extern "C" unsigned int paintcanvas_ext_strlen(void *);
extern "C" int paintcanvas_ext_text_width(void *, unsigned int, unsigned int);

int GetTextWidth(AbyssEngine::PaintCanvas *self, unsigned int index, void *str)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        void *font = (*(void ***)((char *)self + 0x144))[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_text_width(font, len, *(unsigned int *)((char *)str + 0x8));
    }
    return 0;
}
