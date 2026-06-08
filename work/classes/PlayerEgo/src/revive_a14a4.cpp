#include "class.h"

// PlayerEgo::revive()
//   Brings a destroyed player back to life: re-enables its engine particle
//   system, deletes the explosion, re-activates the follow camera and engine
//   sound, restores hitpoints/armor to max, resets position and facing, and
//   clears the explosion timer.

extern "C" void  ParticleSystemManager_enableSystemEmit(int mgr, bool en);
extern "C" void  ParticleSystemManager_enableSystemRender(int mgr, bool en);
extern "C" void *Explosion_dtor(void *exp);
extern "C" void  operator_delete_(void *p);
extern "C" void  TargetFollowCamera_setActive(void *cam, bool active);
extern "C" int   Player_setActive(void *player, bool active);
extern "C" int   FModSound_play(void *snd, void *pos, void *a, int b);
extern "C" void  PlayerEgo_setExhaustVisible(PlayerEgo *self, bool vis);
extern "C" int   Player_getMaxHitpoints(void *player);
extern "C" void  Player_setHitpoints(void *player, int hp);
extern "C" int   Player_getMaxArmorHP(void *player);
extern "C" void  Player_setArmorHP(void *player, int hp);
extern "C" void  PlayerEgo_setPosition(PlayerEgo *self, int a, int b, int c);
extern "C" void  AEGeometry_setDirection(void *geo, const void *dir);

__attribute__((visibility("hidden"))) extern void **g_PE_reviveSound;

extern "C" void PlayerEgo_revive(PlayerEgo *self)
{
    int psm = I(P(self, 0xc), 0x74);
    bool en = C(self, 0x2fc) != 0;
    ParticleSystemManager_enableSystemEmit(psm, en);
    ParticleSystemManager_enableSystemRender(psm, en);

    if (P(self, 0x8c) != 0)
        operator_delete_(Explosion_dtor(P(self, 0x8c)));
    P(self, 0x8c) = 0;

    TargetFollowCamera_setActive(P(self, 0x88), true);
    int v = Player_setActive(P(self, 0x0), true);

    void *snd = *g_PE_reviveSound;
    v = FModSound_play(*(void **)snd, *(void **)snd, 0, v);
    FModSound_play(snd, P(self, 0x1c), 0, v);

    PlayerEgo_setExhaustVisible(self, true);

    void *player = P(self, 0x0);
    Player_setHitpoints(player, Player_getMaxHitpoints(player));
    player = P(self, 0x0);
    Player_setArmorHP(player, Player_getMaxArmorHP(player));

    PlayerEgo_setPosition(self, 0, 0, 0);

    float fwd[3] = {0.0f, 0.0f, 1.0f};
    AEGeometry_setDirection(P(self, 0x8), fwd);
    I(self, 0x2f8) = 0;
}
