#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_MTitle_r3d_canvas;

extern "C" void PaintCanvas_ClearBuffer(void *canvas, int value);
extern "C" void PaintCanvas_Begin3d(void *arg);
extern "C" void MTitle_r3dTail(void *arg);

void MTitle::OnRender3D()
{
    void **canvas = g_MTitle_r3d_canvas;
    PaintCanvas_ClearBuffer(*canvas, 0xff);
    PaintCanvas_Begin3d(this->f_4);
    MTitle_r3dTail(this->f_4);
}
