#include "class.h"

// PlayerEgo::update(int dt, Radar*, Hud*, Radio*, LevelScript*, int, bool, int)
//   The master per-frame tick for the player ship. It runs, in order:
//     1. position cache + non-flight subsystems (shield regen, hacking game,
//        cargo overflow, jump-drive charge, cloak) -- delegated to PE_upd_subsystems,
//     2. engine-overheat / boost handling incl. the destruct explosion when the
//        boost meter overflows -- delegated to PE_upd_boost,
//     3. the asteroid / station docking state dispatch (dockToAsteroid /
//        approachAsteroid / dockToDockingPoint / approachDockingPoint),
//     4. the flight controller: auto-pilot route following or manual flight,
//        turret-view, evasive maneuver, ship handling and the auto-turret,
//     5. collision resolution and post-processing -- delegated to PE_upd_post.
//   The dense numeric phases live in the PE_upd_* helpers; the recoverable
//   high-level dispatch (which decides *which* controller runs) is authored here.

extern "C" int  PlayerEgo_isDead(PlayerEgo *self);
extern "C" int  Player_isActive(void *player);
extern "C" int  Player_getHitpoints(void *player);
extern "C" void Player_stopShooting(void *player);
extern "C" void PlayerEgo_getPosition(PlayerEgo *self, void *out);

// flight controllers (already authored as their own methods).
extern "C" void PlayerEgo_roll(PlayerEgo *self, int amount);
extern "C" int  PlayerEgo_updateManeuver(PlayerEgo *self);
extern "C" void PlayerEgo_handleShip(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_handleTurretView(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_handleAutoTurret(PlayerEgo *self, int dt);
extern "C" void PlayerEgo_dockToAsteroid(PlayerEgo *self, void *radar);
extern "C" void PlayerEgo_dockToDockingPoint(PlayerEgo *self, void *radar);
extern "C" void PlayerEgo_approachAsteroid(PlayerEgo *self, int hud2, void *radar);
extern "C" int  PlayerEgo_approachDockingPoint(PlayerEgo *self, void *hud, int hud2, void *radar);
extern "C" void PlayerEgo_moveToPosition(PlayerEgo *self, float x, float y, float z,
                                         int steer, float speed);
extern "C" void KIPlayer_setAutoPilot(PlayerEgo *self);

// route / nav helpers.
extern "C" void *Route_getWaypoint(void *route);
extern "C" void *Status_getShip();
extern "C" float Ship_getHandling(void *ship);

// dense numeric phases (own the corrupted FP blocks).
extern "C" void PE_upd_subsystems(PlayerEgo *self, int dt, void *radar, void *hud,
                                  void *radio, void *script);
extern "C" void PE_upd_boost(PlayerEgo *self, int dt);
extern "C" void PE_upd_docksFinishDelivery(PlayerEgo *self, void *radio);
extern "C" void PE_upd_post(PlayerEgo *self, int dt, void *radar, void *hud,
                            void *radio, int arg5);

extern const float g_PE_upd_handlingBias;   // 0xae484

extern "C" void PlayerEgo_update(PlayerEgo *self, int dt, void *radar, void *hud,
                                 void *radio, void *script, int arg5, bool arg6, int arg7)
{
    (void)arg6; (void)arg7; (void)script;

    if (P(self, 0x220) == 0)
        return;
    if (C(self, 0x24) != 0)
        return;   // frozen

    // cache world position for this frame.
    float pos[3];
    PlayerEgo_getPosition(self, pos);
    I(self, 0x148) = *(int *)&pos[0];
    I(self, 0x14c) = *(int *)&pos[1];
    I(self, 0x150) = *(int *)&pos[2];

    // shield regen / hacking / cargo / jump-drive / cloak.
    PE_upd_subsystems(self, dt, radar, hud, radio, script);
    // engine overheat + boost (may spawn the destruct explosion at 0x90).
    PE_upd_boost(self, dt);

    // ---- asteroid docking --------------------------------------------------
    if (C(self, 0x1c0) != 0) {
        C(self, 0x145) = 1;
        if (P(self, 0x1bc) == 0 || PlayerEgo_isDead(self) != 0) {
            PlayerEgo_dockToAsteroid(self, 0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        PlayerEgo_approachAsteroid(self, 0 /*hud2*/, radar);
    }

    // ---- station docking ---------------------------------------------------
    if (C(self, 0x356) != 0 && Player_getHitpoints(P(self, 0x0)) > 0) {
        C(self, 0x145) = 1;
        if (P(self, 0x1bc) == 0 || PlayerEgo_isDead(self) != 0) {
            PlayerEgo_dockToDockingPoint(self, 0);
            PE_upd_post(self, dt, radar, hud, radio, arg5);
            return;
        }
        if (PlayerEgo_approachDockingPoint(self, hud, 0, radar) != 0)
            PE_upd_docksFinishDelivery(self, radio);
    }

    // turret view while the free-look/turret camera is active.
    if (C(self, 0x1a0) != 0 || C(self, 0x1a1) != 0)
        PlayerEgo_handleTurretView(self, dt);

    // ---- flight controller -------------------------------------------------
    bool autopilot = (C(self, 0x158) != 0 && I(self, 0x15c) != 0);
    if (!autopilot) {
        if (C(self, 0x145) == 0) {
            if (C(self, 0x1a0) == 0 && C(self, 0x1a1) == 0) {
                PlayerEgo_roll(self, I(self, 0x134));
                if (PlayerEgo_updateManeuver(self) == 0)
                    PlayerEgo_handleShip(self, dt);
            } else {
                PlayerEgo_handleTurretView(self, dt);
            }
        }
    } else if (C(self, 0x145) == 0 && PlayerEgo_updateManeuver(self) == 0) {
        void *wp = *(void **)((char *)self + 0x15c);
        if (C(self, 0x160) != 0 && wp != 0 && I(wp, 0x134) != 0) {
            wp = Route_getWaypoint(*(void **)((char *)self + 0x15c));
            P(self, 0x15c) = wp;
        }
        if (wp == 0 || C(self, 0x1ed) != 0) {
            KIPlayer_setAutoPilot(self);
        } else {
            // steer toward the current waypoint.
            float wpPos[3];
            typedef void (*getpos_fn)(void *, void *);
            (*(getpos_fn *)(*(char **)wp + 0x28))(wp, wpPos);
            I(self, 0xec) = *(int *)&wpPos[0];
            I(self, 0xf0) = *(int *)&wpPos[1];
            I(self, 0xf4) = *(int *)&wpPos[2];

            float me[3];
            PlayerEgo_getPosition(self, me);
            float dx = me[0] - wpPos[0], dy = me[1] - wpPos[1], dz = me[2] - wpPos[2];
            if ((int)(dx * dx + dy * dy + dz * dz) < 20000)
                C(self, 0x330) = 1;

            float speed = 4.0f;
            float h = Ship_getHandling(Status_getShip());
            if (h + g_PE_upd_handlingBias < 4.0f)
                speed = Ship_getHandling(Status_getShip()) + g_PE_upd_handlingBias;
            PlayerEgo_moveToPosition(self, F(self, 0xec), F(self, 0xf0), F(self, 0xf4), 1, speed);
            if (C(self, 0x1a0) != 0)
                PlayerEgo_handleTurretView(self, dt);
        }
    }

    // continue an evasive maneuver during a non-final docking glide.
    if (C(self, 0x356) != 0 && U(self, 0x1c4) != 1
        && C(self, 0x1a0) == 0 && (U(self, 0x1c4) | 1) != 3)
        PlayerEgo_updateManeuver(self);

    // auto-turret (when enabled and the radar window allows it).
    if (C(self, 0x180) != 0 && C(self, 0x355) != 0) {
        if (PlayerEgo_isDead(self) == 0) {
            PlayerEgo_handleAutoTurret(self, dt);
        } else {
            C(self, 0x355) = 0;
            Player_stopShooting(P(self, 0x0));
        }
    }

    // collision + camera + explosion post-processing.
    PE_upd_post(self, dt, radar, hud, radio, arg5);
}
