#include "class.h"

extern "C" int paintcanvas_ext_font_get_spacing(void *);

int FontGetSpacing(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        void *font = (*(void ***)((char *)self + 0x144))[index];
        return paintcanvas_ext_font_get_spacing(font);
    }
    return 0;
}
