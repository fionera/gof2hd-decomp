#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile PaintCanvas_global;
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, uint32_t handle);

bool PlayerJumpgate::animationEnded()
{
    if (F<uint8_t>(this, 0x140) == 0) {
        return false;
    }

    void *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                        F<uint32_t>(this, 0x144));
    return F<uint8_t>(transform, 0xed) == 0;
}
