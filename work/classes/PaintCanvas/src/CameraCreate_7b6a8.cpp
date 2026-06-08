#include "class.h"

// AbyssEngine::PaintCanvas::CameraCreate(unsigned& out) — allocates a Camera sized to the
// current viewport and appends it to the camera array at +0x164, returning its index.
extern "C" int pc_GetWidth(AbyssEngine::PaintCanvas *self);
extern "C" int pc_GetHeight(AbyssEngine::PaintCanvas *self);
extern "C" void pc_Camera_ctor(void *cam, float h, float w);
extern "C" void pc_ArrayAdd_Camera(void *cam, void *arr);

void CameraCreate(AbyssEngine::PaintCanvas *self, unsigned int *out)
{
    void *cam = operator new(0x5c);
    int w = pc_GetWidth(self);
    int h = pc_GetHeight(self);
    pc_Camera_ctor(cam, (float)h, (float)w);
    pc_ArrayAdd_Camera(cam, (char *)self + 0x164);
    *out = *(int *)((char *)self + 0x164) - 1;
}
