#include "class.h"

struct KIPlayer;
struct Player;
struct PlayerTurret;

__attribute__((visibility("hidden"))) extern int *g_cft_stack; // [DAT_000cc3e8]

extern "C" {
int  Level_createStaticObject_cft(Level *self, int wp, int type, int turret);
void Player_setVulnerable_cft(int player, int flag);
int  Player_getMaxHitpoints_cft();
void Player_setMaxHitpoints_cft(Player *p, int hp);
void PlayerTurret_setHost_cft(PlayerTurret *t, KIPlayer *host, void *offset);
void ArrayAdd_KIPlayer_cft(KIPlayer *k, void *arr);
}

// Level::createFighterTurrets() — attaches a defensive turret to capital-class enemies.
void Level::createFighterTurrets()
{
    char *self = (char *)this;
    unsigned *list = *(unsigned **)(self + 0xf8);
    if (list == 0)
        return;

    for (unsigned i = 0; i < *list; i = i + 1) {
        char *ki = *(char **)(list[1] + i * 4);
        if (ki != 0) {
            int kind = *(int *)(ki + 0x7c);
            if (kind == 0x2d || kind == 0x33) {
                PlayerTurret *t = (PlayerTurret *)Level_createStaticObject_cft(this, 0, 0x1a74, 1);
                Player_setVulnerable_cft(*(int *)((char *)t + 4), 0);
                Player *pp = *(Player **)((char *)t + 4);
                int hp = Player_getMaxHitpoints_cft();
                Player_setMaxHitpoints_cft(pp, hp);
                char offset[12] = {0};
                PlayerTurret_setHost_cft(t, (KIPlayer *)ki, offset);
                *(PlayerTurret **)(ki + 0x10) = t;
                *(int *)((char *)t + 0x28) = (kind == 0x2d) ? 8 : 0;
                *(char *)((char *)t + 0x74) = 1;
                ArrayAdd_KIPlayer_cft((KIPlayer *)t, *(void **)(self + 0xf8));
            }
        }
        list = *(unsigned **)(self + 0xf8);
    }
}
