#include "class.h"

extern "C" void *paintcanvas_ext_str_text(void *);
extern "C" int paintcanvas_ext_text_width_range(void *, void *, unsigned int, unsigned int);

int GetTextWidth(AbyssEngine::PaintCanvas *self, unsigned int index, void *str,
                 unsigned int begin, unsigned int end)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        void *font = (*(void ***)((char *)self + 0x144))[index];
        void *text = paintcanvas_ext_str_text(str);
        return paintcanvas_ext_text_width_range(font, text, begin, end - begin);
    }
    return 0;
}
