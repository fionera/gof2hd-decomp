#include "class.h"

extern "C" void paintcanvas_ext_drawtextlines6(void *, unsigned int, void *, int, int, bool);

void DrawTextLines(void *self, unsigned int p1, void *p2, int p3, int p4)
{
    return paintcanvas_ext_drawtextlines6(self, p1, p2, p3, p4, false);
}
