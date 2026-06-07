#include "class.h"

extern "C" void paintcanvas_ext_camera_apply(void *, unsigned int, unsigned int, unsigned int);

void CameraSetCurrent(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    *(unsigned int *)((char *)self + 0x170) = index;
    if (index < *(unsigned int *)((char *)self + 0x164)) {
        unsigned int *cam = (*(unsigned int ***)((char *)self + 0x168))[index];
        return paintcanvas_ext_camera_apply(self, cam[0], cam[1], cam[2]);
    }
}
