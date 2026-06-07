#include "class.h"

extern "C" int paintcanvas_ext_get_w(AbyssEngine::PaintCanvas *);
extern "C" int paintcanvas_ext_get_h(AbyssEngine::PaintCanvas *);
extern "C" void paintcanvas_ext_cam_persp4(float, float, float, float, float, void *);
extern "C" void paintcanvas_ext_cam_setcur(AbyssEngine::PaintCanvas *, unsigned int);

void CameraSetPerspective(AbyssEngine::PaintCanvas *self, unsigned int index, float a, float b, float c)
{
    if (index < *(unsigned int *)((char *)self + 0x164)) {
        float w = (float)paintcanvas_ext_get_w(self);
        float h = (float)paintcanvas_ext_get_h(self);
        void *cam = (*(void ***)((char *)self + 0x168))[index];
        paintcanvas_ext_cam_persp4(a, b, c, w, h, cam);
        if (*(unsigned int *)((char *)self + 0x170) == index) {
            return paintcanvas_ext_cam_setcur(self, index);
        }
    }
}
