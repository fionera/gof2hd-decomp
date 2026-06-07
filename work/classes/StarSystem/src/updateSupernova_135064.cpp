#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_a;
__attribute__((visibility("hidden"))) extern void **g_StarSystem_update_canvas_b;

extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int transform_id);
extern "C" void Transform_Update(void *transform, int supernova, int dt_lo, int dt_hi, int same_mode);

extern "C" void StarSystem_updateSupernova(StarSystem *self, int dt)
{
    void *streak = P(self, 0x40);
    if (streak != 0) {
        void *transform = PaintCanvas_TransformGetTransform(*g_StarSystem_update_canvas_a, I(streak, 0x0c));
        int mode = 0;
        Transform_Update(transform, mode, dt, dt >> 31, mode);
    }
    void *sun = *(void **)P(P(self, 0x1c), 4);
    if (sun != 0) {
        void *transform = PaintCanvas_TransformGetTransform(*g_StarSystem_update_canvas_b, I(sun, 0x0c));
        int mode = 1;
        Transform_Update(transform, mode, dt, dt >> 31, mode);
    }
}
