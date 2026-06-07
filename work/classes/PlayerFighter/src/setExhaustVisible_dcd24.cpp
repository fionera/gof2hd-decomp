#include "class.h"

extern "C" unsigned PaintCanvas_TransformGetTransform(unsigned t);
extern "C" void PlayerFighter_setExhaustVisible_apply(unsigned transform, bool vis);
// hidden PC-relative pointer-to-pointer global (deref'd twice).
extern void *const gExhaustCanvas __attribute__((visibility("hidden")));

extern "C" void PlayerFighter_setExhaustVisible(PlayerFighter *self, bool vis)
{
    int geom = F<int32_t>(self, 0x8);
    if (geom != 0) {
        int sub = F<int32_t>(self, 0xc);
        int id = (sub != 0) ? *(int *)(sub + 0x14) : *(int *)(geom + 0x14);
        if (id != -1) {
            unsigned t = PaintCanvas_TransformGetTransform(*(unsigned *)gExhaustCanvas);
            return PlayerFighter_setExhaustVisible_apply(t, vis);
        }
    }
}
