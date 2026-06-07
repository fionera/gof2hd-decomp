#include "class.h"

extern "C" int paintcanvas_ext_dtl_textwidth(void *, unsigned int, void *);
extern "C" void paintcanvas_ext_dtl_drawstring(void *, unsigned int, void *, int, int, bool);
extern "C" int paintcanvas_ext_dtl_textheight(void *, unsigned int);

void DrawTextLines(AbyssEngine::PaintCanvas *self, unsigned int font,
                   AbyssEngine::Array<void *> *arr, int x, int y, unsigned int p5, bool flag)
{
    int xoff = 0;
    for (unsigned int i = 0; i < arr->size; i++) {
        if (flag == 0) {
            int w = paintcanvas_ext_dtl_textwidth(self, font, arr->data[i]);
            xoff = (int)p5 - w;
        }
        paintcanvas_ext_dtl_drawstring(self, font, arr->data[i], xoff + x, y, false);
        y += paintcanvas_ext_dtl_textheight(self, font);
    }
}
