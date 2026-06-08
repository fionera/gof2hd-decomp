#include "class.h"

// PlayerEgo::left(int frameTime, float delta)
//   Yaw-left input handler. Routes the input depending on mode:
//     * mining game active (0x1e4): forward the (negated) delta to it,
//     * turret mode (0x1a0): rotate the three turret geometries,
//     * maneuver lock (0x194): feed the scripted-maneuver roll instead,
//     * otherwise: set the yaw direction (0x104=+1), compute the load-scaled
//       yaw rate and ramp the yaw accumulator (0x27c) toward the target.
//   The yaw-rate ramp (which samples the active-camera ease state) is computed
//   by PE_yawRampDelta to keep the FP-flag soup out of line.

extern "C" float MiningGame_steerX(void *mg, float d);                 // 0x1abb08 veneer
extern "C" void  AEGeometry_rotateY(void *geo, int a, float ang, int b);// turret yaw veneer
extern "C" void *Status_getShip();
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);
extern "C" int   aeabi_idiv_(int a, int b);
// Ramp helper: given base rate and inputs, returns the accumulator increment.
extern "C" float PE_yawRampDelta(float rate, int frameTime);

extern const float g_PE_l_loadK;    // 0xb0b68
extern const float g_PE_l_loadB;    // 0xb0b6c
extern const float g_PE_l_rateK;    // 0xb0b70
extern const float g_PE_l_turK1;    // 0xb0b54
extern const float g_PE_l_turK2;    // 0xb0b58
extern const float g_PE_l_turK3;    // 0xb0b5c
extern const float g_PE_l_manK1;    // 0xb0b60
extern const float g_PE_l_manK2;    // 0xb0b64

extern "C" float PlayerEgo_left(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0)
        return MiningGame_steerX(P(self, 0x1e4), -delta);

    if (C(self, 0x1a0) != 0) {
        // turret yaw: scale by inverse turret-pitch and apply to 3 nodes.
        float pitch = (float)I(self, 0x1f8);
        float ft = (float)frameTime;
        float ang = ((ft * delta) / (g_PE_l_turK1 / pitch)) * g_PE_l_turK2 * g_PE_l_turK3;
        AEGeometry_rotateY(P(self, 0x178), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0xdc), 0, ang, 0);
        AEGeometry_rotateY(P(self, 0x28), 0, ang, 0);
        return ang;
    }

    if (P(self, 0x194) != 0) {
        float ft = (float)frameTime;
        F(self, 0x80) = delta * g_PE_l_manK1 * F(P(self, 0x194), 0x50);
        F(self, 0x198) = F(self, 0x198) + (ft * delta) * g_PE_l_manK2;
        return ft * delta;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x104) = 1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_l_loadK + F(self, 0x154) * g_PE_l_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_l_rateK * rate), 0x3f);
    F(self, 0x25c) = rate;
    F(self, 0x268) = -delta;
    if (F(self, 0x27c) < target) {
        float step = PE_yawRampDelta(rate, frameTime);
        float v = F(self, 0x27c) + step;
        if (target < v) v = target;
        F(self, 0x27c) = v;
    }
    return target;
}
