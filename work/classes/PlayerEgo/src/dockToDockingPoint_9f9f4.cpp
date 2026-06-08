#include "class.h"

// PlayerEgo::dockToDockingPoint(KIPlayer*, Radar*)
//   Toggles automated docking onto a station docking point. If not docking and
//   given a target, it latches the approach state (0x356/0x1bc/0x1d8). If a
//   target is given while docking, it finds the nearest nav point, switches off
//   the turret/look cameras, and builds an EaseInOutMatrix (0x358) that glides
//   the ship from its current pose to the docking pose over 3000 ms (the 30-arg
//   ctor is wrapped by PE_dtdp_makeEase). With no target it undocks: restores
//   cameras, clears state, frees any reserved SpacePoint, and tears down any
//   running hacking game.

extern "C" int   PlayerEgo_isDead(PlayerEgo *self);
extern "C" void  PlayerEgo_getPosition(PlayerEgo *self, void *out);
extern "C" void *KIPlayer_getNearestNavigationPoint(void *radar, void *posOut, void *spOut);
extern "C" void  PlayEngineSound_(PlayerEgo *self);
extern "C" void  Vec_assign(void *dst, const void *src);
extern "C" void  TargetFollowCamera_setActive(void *cam, bool active);
extern "C" void  TargetFollowCamera_setLookAtCam(void *cam, bool on);
extern "C" void  TargetFollowCamera_useTargetsUpVector(void *cam, bool on);
extern "C" void  Player_resetGunDelay(void *player, int idx);
extern "C" void  PlayerEgo_setExhaustVisible(PlayerEgo *self, bool vis);
extern "C" void  SpacePoint_giveFree(void *sp);
extern "C" void  PlayerEgo_setTurretMode(PlayerEgo *self, int on);
extern "C" void  PaintCanvas_CameraSetCurrent(void *canvas, unsigned int cam);
extern "C" void  LevelScript_resetCamera(void *level);
extern "C" void *EaseInOutMatrix_dtor(void *m);
extern "C" void  operator_delete_(void *p);
extern "C" void *AEGeometry_getMatrix(void *geo);
extern "C" void *HackingGame_dtor(void *hg);
extern "C" void  Hud_setHackingGameActive(void *hud, bool active);
// Wraps operator new + EaseInOutMatrix(from-matrix, to-translation, 3000ms).
extern "C" void *PE_dtdp_makeEase(const void *fromMatrix, const void *navPoint);

__attribute__((visibility("hidden"))) extern void **g_PE_dtdp_canvas;   // 0xafa68 PaintCanvas

extern "C" void PlayerEgo_dockToDockingPoint(PlayerEgo *self, void *radar)
{
    if (PlayerEgo_isDead(self) != 0)
        return;

    if (C(self, 0x356) == 0) {
        if (radar != 0) {
            C(self, 0x356) = 1;
            P(self, 0x1bc) = radar;
            I(self, 0x1d8) = 0x578;
            I(self, 0x1dc) = 0;
            C(self, 0x145) = 1;
            I(self, 0x1c4) = 0;
        }
        return;
    }

    bool undock = (radar == 0);
    if (!undock) {
        float pos[3];
        PlayerEgo_getPosition(self, pos);
        void *sp = (void *)(unsigned long)I(self, 0x36c);
        void *nav = KIPlayer_getNearestNavigationPoint(radar, pos, sp);
        if (nav == 0) {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;
            undock = true;
        } else {
            if (C(radar, 0x70) != 0)
                C(radar, 0x8c) = 1;

            PlayerEgo_setTurretMode(self, 0);
            C(self, 0x1a1) = 0;
            PaintCanvas_CameraSetCurrent(*g_PE_dtdp_canvas, U(P(self, 0x88), 0));
            LevelScript_resetCamera(P(self, 0x10));
            PlayEngineSound_(self);
            I(self, 0x1c4) = 3;
            TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
            TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);

            if (P(self, 0x358) != 0)
                operator_delete_(EaseInOutMatrix_dtor(P(self, 0x358)));
            P(self, 0x358) = 0;

            PlayerEgo_getPosition(self, pos);
            void *nav2 = KIPlayer_getNearestNavigationPoint(radar, pos, sp);
            void *from = AEGeometry_getMatrix(P(self, 0x8));
            P(self, 0x358) = PE_dtdp_makeEase(from, nav2);
            PlayerEgo_setExhaustVisible(self, true);
        }
    }

    if (undock) {
        PlayEngineSound_(self);
        C(self, 0x356) = 0;
        C(self, 0x1a1) = 0;
        C(self, 0x145) = 0;
        P(self, 0x1bc) = 0;
        int zero[3] = {0, 0, 0};
        Vec_assign((char *)self + 0x1c8, zero);
        TargetFollowCamera_setActive(P(self, 0x88), true);
        TargetFollowCamera_setLookAtCam(P(self, 0x88), false);
        TargetFollowCamera_useTargetsUpVector(P(self, 0x88), false);
        Player_resetGunDelay(P(self, 0x0), 0);
        I(self, 0x1c4) = 0;
        PlayerEgo_setExhaustVisible(self, true);
        if (I(self, 0x36c) != 0) {
            SpacePoint_giveFree((void *)(unsigned long)I(self, 0x36c));
            I(self, 0x36c) = 0;
        }
    }

    if (P(self, 0x1e8) != 0) {
        operator_delete_(HackingGame_dtor(P(self, 0x1e8)));
        P(self, 0x1e8) = 0;
        Hud_setHackingGameActive(P(self, 0x220), false);
    }
}
