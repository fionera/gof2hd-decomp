#include "class.h"

extern "C" void paintcanvas_ext_gl_deletetextures(int, void *);

void Suspend(AbyssEngine::PaintCanvas *self)
{
    char texId[4];
    for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x10); i++) {
        int *p = (*(int ***)((char *)self + 0x14))[i];
        *(int *)texId = *p;
        if (*p != -1) {
            paintcanvas_ext_gl_deletetextures(1, texId);
            p = (*(int ***)((char *)self + 0x14))[i];
        }
        *p = -1;
    }
}
