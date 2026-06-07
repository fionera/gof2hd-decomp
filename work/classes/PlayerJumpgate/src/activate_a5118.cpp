#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile PaintCanvas_global;
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, uint32_t handle);
extern "C" void Transform_SetAnimationState(void *transform, int state, int loop);
extern "C" void PaintCanvas_TransformRemoveChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" void AEGeometry_addChild(void *geometry, uint32_t handle);

void PlayerJumpgate::activate()
{
    if (F<uint8_t>(this, 0x140) != 0) {
        return;
    }

    uint32_t handle = F<uint32_t>(this, 0x144);
    if (handle != 0xffffffffU) {
        void **canvasOwner = (void **)PaintCanvas_global;
        void *transform = PaintCanvas_TransformGetTransform(*canvasOwner, handle);
        Transform_SetAnimationState(transform, 1, 0);

        void *geometry = F<void *>(this, 0x8);
        PaintCanvas_TransformRemoveChild(*canvasOwner, F<uint32_t>(geometry, 0xc),
                                         F<uint32_t>(geometry, 0x14));
        AEGeometry_addChild(F<void *>(this, 0x8), F<uint32_t>(this, 0x144));
    }

    F<uint8_t>(this, 0x140) = 1;
}
