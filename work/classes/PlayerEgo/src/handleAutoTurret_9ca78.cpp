#include "class.h"

// PlayerEgo::handleAutoTurret(int dt)
//   Drives the automatic gun turret. Every 3 seconds (timer 0x184) it rescans
//   the level enemies and locks the nearest living, active, hostile target
//   (0x18c). Each frame, if a target is held, it aims the turret geometries at
//   the predicted intercept point and -- when on target and the firing window
//   permits -- fires a turret shot and pulses the muzzle transform. Losing the
//   target or exceeding the no-target window stops shooting. The aim/intercept
//   vector-and-matrix math is delegated to PE_hat_aimAndFire.

extern "C" void *Level_getEnemies(void *level);
extern "C" int   KIPlayer_isDead(void *e);
extern "C" int   KIPlayer_isDying(void *e);
extern "C" int   Player_isActive(void *p);
extern "C" int   KIPlayer_isEnemy(void *e);
extern "C" void  KIPlayer_getPosition(void *e, void *out);   // virtual +0x28
extern "C" void  Vec_sub(void *out, const void *a, const void *b);
extern "C" float Vec_length(const void *v);
extern "C" void  Player_stopShooting(void *player);
// Aims the turret at the held target and fires when appropriate; returns the
// new "no-target" timer contribution (0 while actively tracking).
extern "C" int   PE_hat_aimAndFire(PlayerEgo *self, int dt);

extern "C" void PlayerEgo_handleAutoTurret(PlayerEgo *self, int dt)
{
    int t = I(self, 0x184) + dt;
    I(self, 0x184) = t;
    if (t >= 0xbb9) {                       // 3000ms: rescan for the best target
        I(self, 0x184) = 0;
        P(self, 0x18c) = 0;
        unsigned int *enemies = (unsigned int *)Level_getEnemies(P(self, 0xc));
        if (enemies != 0) {
            int best = 60000;
            for (unsigned i = 0; i < enemies[0]; i++) {
                void *e = *(void **)(enemies[1] + i * 4);
                if (KIPlayer_isDead(e) != 0) continue;
                if (KIPlayer_isDying(e) != 0) continue;
                if (Player_isActive(*(void **)((char *)e + 4)) == 0) continue;
                if (KIPlayer_isEnemy(e) == 0) continue;
                if (C(e, 0x74) != 0) continue;

                float epos[3];
                KIPlayer_getPosition(e, epos);
                float rel[3];
                Vec_sub(rel, epos, (char *)self + 0x148);
                int dist = (int)Vec_length(rel);
                if (dist < best
                    && (P(self, 0x18c) == 0 || P(self, 0x18c) != P(self, 0x190))) {
                    P(self, 0x18c) = e;
                    best = dist;
                }
            }
        }
    }

    void *target = P(self, 0x18c);
    if (target != 0 && I(target, 0x8) != 0) {
        int fireTimer = PE_hat_aimAndFire(self, dt);
        I(self, 0x188) = fireTimer;
        if (fireTimer + dt > 0x1f4) {
            // fall through to stop-shooting below
        } else {
            return;
        }
    }

    Player_stopShooting(P(self, 0x0));
}
