#include "class.h"

extern "C" unsigned int paintcanvas_ext_strlen(void *);
extern "C" void paintcanvas_ext_check_string(void *, unsigned int, unsigned int);

void CheckString(AbyssEngine::PaintCanvas *self, unsigned int index, void *str)
{
    if (index < *(unsigned int *)((char *)self + 0x140)) {
        void *font = (*(void ***)((char *)self + 0x144))[index];
        unsigned int len = paintcanvas_ext_strlen(str);
        return paintcanvas_ext_check_string(font, len, *(unsigned int *)((char *)str + 0x8));
    }
}
