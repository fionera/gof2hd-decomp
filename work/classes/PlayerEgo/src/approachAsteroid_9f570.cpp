#include "class.h"

// PlayerEgo::approachAsteroid(Hud*, int hud2, Radar*)
//   Two-phase asteroid mining controller, dispatched on the dock state 0x1c4:
//     * state 0 (approach): flies toward the asteroid's surface point, ramps
//       the throttle down on arrival, kills the exhaust, and aligns the hull to
//       the surface (the alignment matrix math is in PE_aa_align). Once docked
//       it switches to state 1.
//     * state 1 (mining): once the settle timer (0x1dc) elapses it runs the
//       mining mini-game (0x1e4) -- creating it on first entry -- and reacts to
//       win / loss / death by stopping the mine and firing the HUD event.
//   Skipped entirely while dying. The dense approach physics is delegated to
//   PE_aa_approachStep; the recoverable state machine stays inline.

extern "C" int   KIPlayer_isDying(void *self);
extern "C" int   KIPlayer_isDead(void *self);
extern "C" void *MiningGame_new(int quality, int seed, void *hud);
extern "C" int   MiningGame_update(void *mg, int dt);
extern "C" int   MiningGame_gameLost(void *mg);
extern "C" int   MiningGame_getOreAmount(void *mg);
extern "C" int   PlayerAsteroid_getQuality(void *ast);
extern "C" int   FModSound_play(int snd, int id, void *a, int b);
extern "C" void  FModSound_pause(void *snd);
extern "C" void  PlayerEgo_stopMining(PlayerEgo *self);
extern "C" void  Hud_hudEvent(void *hud, int ev, void *arg);
// Owns the corrupted approach steering + hull-alignment matrix math; returns the
// new dock state (0 = still approaching, 1 = docked) and updates fields in place.
extern "C" int   PE_aa_approachStep(PlayerEgo *self, int hud2, void *radar);

__attribute__((visibility("hidden"))) extern void **g_PE_aa_levelHolder; // 0xaf8ec asteroid host
__attribute__((visibility("hidden"))) extern int  *g_PE_aa_mineSound;    // 0xaf928
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder1;  // 0xaf984
__attribute__((visibility("hidden"))) extern void **g_PE_aa_winHolder2;  // 0xaf742
extern const float g_PE_aa_settleEps;   // 0xaf9c0

extern "C" void PlayerEgo_approachAsteroid(PlayerEgo *self, int hud2, void *radar)
{
    if (KIPlayer_isDying(self) != 0)
        return;

    if (I(self, 0x1c4) == 1) {
        // mining phase: wait for the settle timer, then run the mini-game.
        float settle = F(self, 0x1dc);
        if (settle < g_PE_aa_settleEps) {
            F(self, 0x1dc) = settle + (float)(-(I(self, 0x134)) >> 1);
            return;
        }

        if (P(self, 0x1e4) == 0) {
            C(self, 0x2f5) = 0;
            C(self, 0x39b) = 0;
            C(*g_PE_aa_levelHolder, 0x37) = 0;
            void *mg = MiningGame_new(PlayerAsteroid_getQuality(P(self, 0x1bc)),
                                      I(P(self, 0x1bc), 0x128), (void *)(unsigned long)hud2);
            P(self, 0x1e4) = mg;
            I((void *)(I(self, 0x1bc) + 4), 0x40) = 0;
            int snd = *g_PE_aa_mineSound;
            FModSound_play(snd, 1, 0, 0);
            FModSound_pause((void *)(unsigned long)snd);
            return;
        }

        int running = MiningGame_update(P(self, 0x1e4), I(self, 0x134));
        if (running == 0) {
            if (MiningGame_gameLost(P(self, 0x1e4)) == 0
                && MiningGame_getOreAmount(P(self, 0x1e4)) > 0) {
                PlayerEgo_stopMining(self);
            } else if (MiningGame_gameLost(P(self, 0x1e4)) != 0) {
                C(self, 0x39b) = 1;
                I(*g_PE_aa_winHolder1, 0x124) = 0;
                PlayerEgo_stopMining(self);
                Hud_hudEvent((void *)(unsigned long)hud2, 8, self);
            }
        } else if (KIPlayer_isDying(self) != 0 || KIPlayer_isDead(self) != 0) {
            I(*g_PE_aa_winHolder2, 0x124) = 0;
            PlayerEgo_stopMining(self);
            Hud_hudEvent((void *)(unsigned long)hud2, 8, self);
        }
        return;
    }

    if (I(self, 0x1c4) == 0) {
        // approach phase: steer/align toward the asteroid.
        I(self, 0x1c4) = PE_aa_approachStep(self, hud2, radar);
    }
}
