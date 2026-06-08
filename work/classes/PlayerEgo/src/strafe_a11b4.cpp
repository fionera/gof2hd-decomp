#include "class.h"

// PlayerEgo::strafe(int dir, bool positive)
//   Sets the lateral strafe acceleration (0x37c) and clamps the strafe target
//   speed (0x380). When the ship is cargo-loaded (flag 0x235) the base strafe
//   rate (0x154) is reduced proportionally to the current/max load ratio.
//   A maneuver lock at 0x194 suppresses strafing entirely.

extern "C" void *Status_getShip();
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);

extern const float g_PE_strafeLoadK;   // 0xb127c
extern const float g_PE_strafeLoadB;   // 0xb1280
extern const float g_PE_strafeAccelK;  // 0xb1284

static inline float fmin_(float a, float b) { return a < b ? a : b; }

extern "C" void PlayerEgo_strafe(PlayerEgo *self, int /*dir*/, bool positive)
{
    if (I(self, 0x194) != 0)
        return;

    float base;
    if (C(self, 0x235) != 0) {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
        float rate = F(self, 0x154);
        base = rate * (1.0f - cur / max) * g_PE_strafeLoadK + rate * g_PE_strafeLoadB;
    } else {
        base = F(self, 0x154);
    }

    float sign = positive ? 1.0f : -1.0f;
    float accel = fmin_(base * 30.0f * g_PE_strafeAccelK, 2.0f);
    float target = fmin_(F(self, 0x380) * 1.5f, 1.0f);

    F(self, 0x37c) = F(self, 0x380) * sign * accel;
    F(self, 0x380) = target;
}
