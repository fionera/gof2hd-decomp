#include "class.h"

// PlayerEgo::down(int frameTime, float delta)
//   Pitch-down input handler. Dispatch mirrors left()/right() but on the pitch
//   axis: mining game (delegated, with a global gate selecting which veneer),
//   turret free-look (accumulating into the look-pitch 0x1a8 and the turret
//   pitch 0x1a4, each rotating its geometry), scripted maneuver (0x7c), or the
//   normal load-scaled pitch ramp on accumulator 0x278 (direction flag 0x100=-1).

extern "C" float MiningGame_steerY(void *mg, float d);                 // 0x1abb18 veneer
extern "C" float MiningGame_steerYAlt(void);                          // 0x1abb38 veneer
extern "C" void  AEGeometry_rotateX(void *geo, float ang, int a, float b); // 0x72160
extern "C" float AEGeometry_rotateXr(void *geo, float ang, int a, int b);  // 0x1abb28 veneer
extern "C" void *Status_getShip();
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" float PE_pitchRampDelta(PlayerEgo *self, float rate, int frameTime);

__attribute__((visibility("hidden"))) extern char **g_PE_d_miningGate; // 0xb0bb0 -> flags (+0x10)
extern const float g_PE_d_eps;      // 0xb0dc0 free-look limit
extern const float g_PE_d_lookK1;   // 0xb0dc4
extern const float g_PE_d_lookK2;   // 0xb0dcc
extern const float g_PE_d_manK;     // 0xb0dd0
extern const float g_PE_d_loadK;    // 0xb0dd4
extern const float g_PE_d_loadB;    // 0xb0dd8
extern const float g_PE_d_rateK;    // 0xb0ddc

extern "C" float PlayerEgo_down(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_d_miningGate, 0x10) == 0)
            return MiningGame_steerYAlt();
        return MiningGame_steerY(P(self, 0x1e4), -delta);
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) < g_PE_d_eps) {
            float ang = ft * delta + F(self, 0x1a8);
            F(self, 0x1a8) = ang;
            AEGeometry_rotateX(P(self, 0x28), ang * g_PE_d_lookK1 * g_PE_d_lookK2, 0, 0);
        }
        float p = F(self, 0x1a4);
        if (p < g_PE_d_eps) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_d_lookK1 * g_PE_d_lookK2;
            F(self, 0x1a4) = half + p;
            return AEGeometry_rotateXr(P(self, 0x19c), ang, 0, 0);
        }
        return p;
    }

    if (P(self, 0x194) != 0) {
        float v = (float)frameTime * g_PE_d_manK * F(P(self, 0x194), 0x50);
        F(self, 0x7c) = v;
        return v;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x100) = -1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_d_loadK + F(self, 0x154) * g_PE_d_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_d_rateK * rate), 0x3f);
    F(self, 0x270) = delta;
    F(self, 0x258) = -rate;
    if (F(self, 0x278) < target) {
        float v = F(self, 0x278) + PE_pitchRampDelta(self, rate, frameTime);
        if (target < v) v = target;
        F(self, 0x278) = v;
    }
    return target;
}
