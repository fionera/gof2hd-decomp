#include "class.h"

struct Engine;
struct PlayerEgo;

extern "C" int ApplicationManager_GetEngine();                    // 0x7258c
extern "C" int Engine_GetAccelValue(Engine *e);                   // 0x6ff88
extern "C" void PlayerEgo_left(PlayerEgo *p, float amt);          // 0x729d0
extern "C" void PlayerEgo_right(PlayerEgo *p, float amt);         // 0x729dc
// Roll-control callbacks invoked by fn-ptr: (ego, shipField, amt).
extern "C" void *MGame_accelCtxBegin(int field8);                 // fn @0x1887b4 (pcVar5)
extern "C" double *MGame_accelCtxValue();                         // fn @0x1887bc (pcVar4)
// Roll tail helpers @0x1abb78 / @0x1abb68.
extern "C" void PlayerEgo_rollRight(PlayerEgo *p, int shipField, float amt);
extern "C" void PlayerEgo_rollLeft(PlayerEgo *p, int shipField, float amt);

__attribute__((visibility("hidden"))) extern int g_accelTune;   // @0x1887d4 (*(int*) iVar6)

// MGame::handleAccelerometer(): tilt-steer the ship from device accelerometer input.
extern "C" void MGame_handleAccelerometer(MGame *self) {
    Engine *eng = (Engine *)ApplicationManager_GetEngine();
    int acc = Engine_GetAccelValue(eng);

    // Yaw from raw tilt (axis at +8, scaled by 2.5).
    float yaw = (float)(*(double *)((char *)acc + 8) * 2.5);
    float steer = 1.0f;
    if (yaw <= 1.0f) {
        steer = -1.0f;
        if (yaw < -1.0f) {
            PlayerEgo_left(F<PlayerEgo *>(self, 0x58), steer * steer);
            goto afterYaw;
        }
        steer = yaw;
        if (yaw < 0.0f) {
            PlayerEgo_left(F<PlayerEgo *>(self, 0x58), steer * steer);
            goto afterYaw;
        }
        if (yaw == 0.0f)
            goto afterYaw;
    }
    PlayerEgo_right(F<PlayerEgo *>(self, 0x58), steer * steer);

afterYaw: {
    int field8 = F<int>(self, 0x8);

    MGame_accelCtxBegin(field8);
    double *v1 = MGame_accelCtxValue();
    double d0 = *v1;
    MGame_accelCtxBegin(field8);
    int c2 = (int)(long)MGame_accelCtxValue();

    float base = (float)d0;
    int tune = g_accelTune;
    double dz = *(double *)((char *)c2 + 0x10);
    float ref = *(float *)((char *)tune + 0x1c);
    char sign = *(char *)((char *)tune + 0x10);

    MGame_accelCtxBegin(field8);
    int c3 = (int)(long)MGame_accelCtxValue();

    float a, b;
    if (sign == 0) {
        float t = base;
        if (*(double *)((char *)c3 + 0x10) > 0.0) {
            t = 1.0f;
            if (base <= 1.0f) t = (1.0f - base) + 1.0f;
        }
        a = ref - t;
        b = *(float *)((char *)tune + 0x20) - (float)dz;
    } else {
        float t = base;
        if (*(double *)((char *)c3 + 0x10) > 0.0) {
            t = 1.0f;
            if (base <= 1.0f) t = (1.0f - base) + 1.0f;
        }
        a = t - ref;
        b = (float)dz - *(float *)((char *)tune + 0x20);
    }

    float a3 = a * 3.0f;
    float b3 = b * 3.0f;
    float aAbs = a * -3.0f;
    if (a3 > 0.0f) aAbs = a3;
    float bAbs = b * -3.0f;
    if (b3 > 0.0f) bAbs = b3;
    float roll = (aAbs < bAbs) ? b3 : a3;

    float rollMag = 1.0f;
    int shipOff = (F<int>(self, 0x16c) > 0) ? 0x44 : 0x40;
    if (roll <= 1.0f) {
        rollMag = -1.0f;
        if (roll < -1.0f) {
            return PlayerEgo_rollRight(F<PlayerEgo *>(self, 0x58),
                                       F<int>(self, shipOff), rollMag * rollMag);
        }
        rollMag = roll;
        if (roll < 0.0f) {
            return PlayerEgo_rollRight(F<PlayerEgo *>(self, 0x58),
                                       F<int>(self, shipOff), rollMag * rollMag);
        }
        if (roll == 0.0f) return;
    }
    return PlayerEgo_rollLeft(F<PlayerEgo *>(self, 0x58),
                              F<int>(self, shipOff), rollMag * rollMag);
}
}
