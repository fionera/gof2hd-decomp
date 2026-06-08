#include "class.h"

// PlayerEgo::right(int frameTime, float delta)
//   Yaw-right input handler -- the mirror of left(): same mode dispatch but the
//   yaw direction flag (0x104) is -1 and the yaw accumulator (0x27c) ramps
//   downward toward the (negative) target.

extern "C" float MiningGame_steerXR(void *mg, float d);                // 0x1abaf8 veneer
extern "C" void  AEGeometry_rotateY(void *geo, int a, float ang, int b);
extern "C" void *Status_getShip();
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" float PE_yawRampDelta(float rate, int frameTime);

extern const float g_PE_r_loadK;    // 0xb0924
extern const float g_PE_r_loadB;    // 0xb0928
extern const float g_PE_r_rateK;    // 0xb092c
extern const float g_PE_r_turK1;    // 0xb0910
extern const float g_PE_r_turK2;    // 0xb0914
extern const float g_PE_r_turK3;    // 0xb0918
extern const float g_PE_r_manK1;    // 0xb091c
extern const float g_PE_r_manK2;    // 0xb0920

extern "C" float PlayerEgo_right(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0)
        return MiningGame_steerXR(P(self, 0x1e4), delta);

    if (C(self, 0x1a0) != 0) {
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_r_turK1 / pitch)) * g_PE_r_turK2 * g_PE_r_turK3;
        AEGeometry_rotateY(P(self, 0x178), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0xdc), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0x28), 0, ang, 0);
        return ang;
    }

    if (P(self, 0x194) != 0) {
        float ft = (float)frameTime;
        F(self, 0x80) = delta * g_PE_r_manK1 * F(P(self, 0x194), 0x50);
        F(self, 0x198) = F(self, 0x198) + (ft * delta) * g_PE_r_manK2;
        return ft * delta;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x104) = -1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_r_loadK + F(self, 0x154) * g_PE_r_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_r_rateK * rate), 0x3f);
    F(self, 0x268) = delta;
    F(self, 0x25c) = -rate;
    if (F(self, 0x27c) > target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = F(self, 0x27c) - step;
        if (v < target) v = target;
        F(self, 0x27c) = v;
    }
    return target;
}
