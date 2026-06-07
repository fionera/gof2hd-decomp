#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile PaintCanvas_global;
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, uint32_t handle);

bool PlayerJumpgate::timeToJump()
{
    void *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                        F<uint32_t>(this, 0x144));
    return F<int64_t>(transform, 0x110) > 1000LL;
}
