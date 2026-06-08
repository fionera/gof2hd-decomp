#include "class.h"

// PlayerEgo::dockToAsteroid(KIPlayer*, Radar*)
//   Toggles the asteroid mining dock. If not docked and given an asteroid, it
//   latches on (records the asteroid at 0x1bc, computes an approach distance
//   from its scaling at 0x1d8). If already docked, it detaches: re-enables the
//   asteroid spin, clears the dock vector (0x1c8), re-activates the follow
//   camera, tears down the mining game (0x1e4) and shows the exhaust again.

extern "C" void  PlayerAsteroid_setRotationEnabled(void *ast, bool en);
extern "C" float PlayerAsteroid_getScaling(void *ast);
extern "C" void  Vec_assign(void *dst, const void *src);                 // Vector::operator=
extern "C" void  TargetFollowCamera_setActive(void *cam, bool active);
extern "C" void  Player_resetGunDelay(void *player, int idx);
extern "C" void *MiningGame_dtor(void *mg);
extern "C" void  operator_delete_(void *p);
extern "C" void  Radar_unlockAsteroid(void *radar);
extern "C" void  PlayerEgo_setExhaustVisible(PlayerEgo *self, bool visible);

extern const float g_PE_astApproach;    // 0xaf568 scaling -> approach distance

extern "C" void PlayerEgo_dockToAsteroid(PlayerEgo *self, void *radar)
{
    if (C(self, 0x1c0) != 0) {
        // Already docked -> undock.
        PlayerAsteroid_setRotationEnabled(P(self, 0x1bc), true);
        C(self, 0x145) = 0;
        C(self, 0x1c0) = 0;
        P(self, 0x1bc) = 0;

        int zero[3] = {0, 0, 0};
        Vec_assign((char *)self + 0x1c8, zero);

        TargetFollowCamera_setActive(P(self, 0x88), true);
        Player_resetGunDelay(P(self, 0x0), 0);
        if (P(self, 0x1e4) != 0)
            operator_delete_(MiningGame_dtor(P(self, 0x1e4)));
        P(self, 0x1e4) = 0;
        Radar_unlockAsteroid(radar);
        I(self, 0x1c4) = 0;
        PlayerEgo_setExhaustVisible(self, true);
    } else if (radar != 0) {
        // Not docked -> latch onto the asteroid.
        C(self, 0x1c0) = 1;
        I(self, 0x1dc) = 0;
        P(self, 0x1bc) = radar;
        float dist = PlayerAsteroid_getScaling(radar) * g_PE_astApproach;
        C(self, 0x145) = 1;
        I(self, 0x1c4) = 0;
        I(self, 0x1d8) = (int)dist;
    }
}
