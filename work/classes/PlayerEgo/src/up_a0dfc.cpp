#include "class.h"

// PlayerEgo::up(int frameTime, float delta)
//   Pitch-up input handler -- the mirror of down(): same axis and dispatch but
//   the free-look/turret accumulators (0x1a8 / 0x1a4) and the pitch accumulator
//   (0x278) ramp upward, with the pitch direction flag (0x100) set to +1.

extern "C" float MiningGame_steerY(void *mg, float d);                 // 0x1abb18 veneer
extern "C" float MiningGame_steerYAlt(void);                          // 0x1abb38 veneer
extern "C" void  AEGeometry_rotateX(void *geo, float ang, int a, float b); // 0x72160
extern "C" float AEGeometry_rotateXr(void *geo, float ang, int a, int b);  // 0x1abb28 veneer
extern "C" void *Status_getShip();
extern "C" int   Ship_getCurrentLoad(void *ship);
extern "C" int   Ship_getMaxLoad(void *ship);
extern "C" int   aeabi_idiv_(int a, int b);
extern "C" float PE_pitchRampDelta(PlayerEgo *self, float rate, int frameTime);

__attribute__((visibility("hidden"))) extern char **g_PE_u_miningGate; // 0xb0e1c -> flags (+0x10)
extern const float g_PE_u_eps;      // 0xb1030 free-look limit
extern const float g_PE_u_eps2;     // 0xb103c
extern const float g_PE_u_lookK1;   // 0xb1034
extern const float g_PE_u_lookK2;   // 0xb1040
extern const float g_PE_u_manK;     // 0xb1044
extern const float g_PE_u_loadK;    // 0xb1048
extern const float g_PE_u_loadB;    // 0xb104c
extern const float g_PE_u_rateK;    // 0xb1050

extern "C" float PlayerEgo_up(PlayerEgo *self, int frameTime, float delta)
{
    if (P(self, 0x1e4) != 0) {
        if (C(*g_PE_u_miningGate, 0x10) == 0)
            return MiningGame_steerY(P(self, 0x1e4), -delta);
        return MiningGame_steerYAlt();
    }

    if (C(self, 0x1a0) != 0) {
        float ft = (float)frameTime;
        if (F(self, 0x1a8) > g_PE_u_eps) {
            float ang = F(self, 0x1a8) - ft * delta;
            F(self, 0x1a8) = ang;
            AEGeometry_rotateX(P(self, 0x28), ang * g_PE_u_lookK1 * g_PE_u_lookK2, 0, 0);
        }
        float p = F(self, 0x1a4);
        if (p > g_PE_u_eps2) {
            float half = ft * delta * 0.5f;
            float ang = half * g_PE_u_lookK1 * g_PE_u_lookK2;
            F(self, 0x1a4) = p - half;
            return AEGeometry_rotateXr(P(self, 0x19c), ang, 0, 0);
        }
        return p;
    }

    if (P(self, 0x194) != 0) {
        float v = (float)frameTime * g_PE_u_manK * F(P(self, 0x194), 0x50);
        F(self, 0x7c) = v;
        return v;
    }

    if (C(self, 0x158) != 0)
        return delta;
    if (C(self, 0x356) != 0 && I(self, 0x1c4) != 1)
        return delta;

    I(self, 0x100) = 1;
    float rate;
    if (C(self, 0x235) == 0) {
        rate = F(self, 0x154);
    } else {
        float cur = (float)Ship_getCurrentLoad(Status_getShip());
        float max = (float)Ship_getMaxLoad(Status_getShip());
        rate = F(self, 0x154) * (1.0f - cur / max) * g_PE_u_loadK + F(self, 0x154) * g_PE_u_loadB;
    }

    float target = (float)aeabi_idiv_((int)(delta * g_PE_u_rateK * rate), 0x3f);
    F(self, 0x258) = rate;
    F(self, 0x270) = -delta;
    if (F(self, 0x278) > target) {
        float v = F(self, 0x278) - PE_pitchRampDelta(self, rate, frameTime);
        if (v < target) v = target;
        F(self, 0x278) = v;
    }
    return target;
}
