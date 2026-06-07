#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile PaintCanvas_global;
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, uint32_t handle);
struct Transform {
    void Update(bool active, int64_t delta, bool enabled);
};

void PlayerJumpgate::update(int delta)
{
    if (F<uint8_t>(this, 0xf5) != 0) {
        void *geometry = F<void *>(this, 0x8);
        void *transform = PaintCanvas_TransformGetTransform(*(void **)PaintCanvas_global,
                                                            F<uint32_t>(geometry, 0xc));
        bool active = true;
        int64_t wideDelta = delta;
        ((Transform *)transform)->Update(active, wideDelta, active);
    }
}
