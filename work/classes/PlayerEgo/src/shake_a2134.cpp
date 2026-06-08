#include "class.h"

// PlayerEgo::shake(int amount)
//   Produces a random camera-shake offset whose magnitude scales with `amount`
//   and the current shake intensity (field 0x134), then applies it to the
//   target-follow camera (field 0x8) via the camera shake entry point.

extern "C" int  AERandom_next(void *rng, int bound);
extern "C" void Camera_shake(void *cam, float dx, float dy, float dz);

__attribute__((visibility("hidden"))) extern void **g_PE_rng;        // AERandom singleton
extern const float g_PE_shakeDiv;                                    // 0xb21d4 normaliser

extern "C" void PlayerEgo_shake(PlayerEgo *self, int amount)
{
    void *cam = P(self, 0x8);
    float a = (float)amount / g_PE_shakeDiv;
    float intensity = (float)(I(self, 0x134) << 1);

    int range = (int)(a * intensity);
    if (range < 2)
        range = 1;
    int span = range << 1;

    void *rng = *g_PE_rng;
    float dx = (float)(AERandom_next(rng, span) - range);
    float dy = (float)(AERandom_next(rng, span) - range);
    float dz = (float)(AERandom_next(rng, span) - range);
    Camera_shake(cam, dx, dy, dz);
}
