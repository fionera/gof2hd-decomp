#include "class.h"

struct PlayerEgo;
struct TargetFollowCamera;
struct LevelScript;
struct Level;

extern "C" int PlayerEgo_isDockingToAsteroid(PlayerEgo *p);            // 0x76804
extern "C" int PlayerEgo_setTurretMode(PlayerEgo *p, int on);         // 0x72b08
extern "C" int PlayerEgo_isDockedToDockingPoint(PlayerEgo *p);        // 0x75928
extern "C" void LevelScript_lookBehind(LevelScript *s, int a);        // 0x7246c
extern "C" void TFC_setRotationAroundTarget(TargetFollowCamera *c, int v); // 0x76c0c
extern "C" void PlayerEgo_setFreeLookMode(PlayerEgo *p, int v);       // 0x76b94
extern "C" int ApplicationManager_GetEngine();                        // 0x7258c
extern "C" void LevelScript_resetCamera(Level *l);                    // 0x76cfc
extern "C" void TFC_setLookAtCam(TargetFollowCamera *c, int v);       // 0x72670
extern "C" void TFC_enableFirstPersonCam(TargetFollowCamera *c, int v); // 0x76ba0
extern "C" int TFC_hideShipForFirstPersonCam(TargetFollowCamera *c);  // 0x78460
// PlayerEgo turret/first-person sync tail helper @0x1ac868.
extern "C" void PlayerEgo_syncFirstPerson(PlayerEgo *p, int v);

// MGame::switchCamera(int id)
extern "C" void MGame_switchCamera(MGame *self, int id) {
    int dockState = 0;
    int turretArg;

restart:
    if (id == 2) id = 3;
    F<int>(self, 0x14) = id;
    if (id == 1) {
        if (PlayerEgo_isDockingToAsteroid(F<PlayerEgo *>(self, 0x58)) != 0) {
            F<uint8_t>(self, 0xd5) = 0;
            // fallthrough to step++
        } else {
            int t = PlayerEgo_setTurretMode(F<PlayerEgo *>(self, 0x58), 1);
            F<uint8_t>(self, 0xd5) = (uint8_t)t;
            if (t != 0) {
                id = F<int>(self, 0x14);
                turretArg = 1;
                goto afterTurret;
            }
        }
        id = F<int>(self, 0x14) + 1;
        F<int>(self, 0x14) = id;
        turretArg = 0;
    } else {
        F<uint8_t>(self, 0xd5) = 0;
        turretArg = 0;
    }

afterTurret:
    if (id == 2) {
        F<int>(self, 0x14) = 3;
    } else if (id >= 4) {
        F<int>(self, 0x14) = 0;
    }
    F<uint8_t>(self, 0x18) = 0;
    PlayerEgo_setTurretMode(F<PlayerEgo *>(self, 0x58), turretArg);

    int mode = F<int>(self, 0x14);
    switch (mode) {
    case 0:
        break;
    case 1:
    case 3: {
        if (PlayerEgo_isDockedToDockingPoint(F<PlayerEgo *>(self, 0x58)) != 0)
            TFC_setLookAtCam(F<TargetFollowCamera *>(self, 0xf4), 0);
        LevelScript_lookBehind(F<LevelScript *>(self, 0x7c), F<int>(self, 0x78));
        TFC_setRotationAroundTarget(F<TargetFollowCamera *>(self, 0xf4), 1);
        PlayerEgo_setFreeLookMode(F<PlayerEgo *>(self, 0x58), 1);
        goto firstPerson;
    }
    case 2: {
        dockState = PlayerEgo_isDockedToDockingPoint(F<PlayerEgo *>(self, 0x58));
        if (dockState == 0) {
            LevelScript_lookBehind(F<LevelScript *>(self, 0x7c), F<int>(self, 0x78));
            TFC_setRotationAroundTarget(F<TargetFollowCamera *>(self, 0xf4), 0);
            PlayerEgo_setFreeLookMode(F<PlayerEgo *>(self, 0x58), 0);
            int eng = ApplicationManager_GetEngine();
            *(int *)((char *)eng + 0x360) = 0;
            goto firstPerson;
        }
        // docked: restart the whole selection with id=1.
        id = 1;
        goto restart;
    }
    default:
        goto firstPerson;
    }

firstPerson: {
        TFC_enableFirstPersonCam(F<TargetFollowCamera *>(self, 0xf4), F<int>(self, 0x14) == 2);
        PlayerEgo *ego = F<PlayerEgo *>(self, 0x58);
        int v;
        if (F<uint8_t>(self, 0x18) == 0)
            v = TFC_hideShipForFirstPersonCam(F<TargetFollowCamera *>(self, 0xf4));
        else
            v = 1;
        PlayerEgo_syncFirstPerson(ego, v);
    }
}
