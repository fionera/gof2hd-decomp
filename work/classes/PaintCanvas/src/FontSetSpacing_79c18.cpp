#include "class.h"

extern "C" void paintcanvas_ext_font_set_spacing(void *, int);

void FontSetSpacing(AbyssEngine::PaintCanvas *self, unsigned int index, short spacing)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        void *font = (*(void ***)((char *)self + 0x144))[index];
        return paintcanvas_ext_font_set_spacing(font, spacing);
    }
}
