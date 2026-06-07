#include "class.h"

struct FModSound;
struct PlayerEgo;
struct KIPlayer;
struct Level;
struct EnemyList { uint32_t size; KIPlayer **data; };

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;
extern "C" void FModSound_resumeAll(FModSound *s);
extern "C" void PlayerEgo_ResumeEngineSound(PlayerEgo *p);
extern "C" EnemyList *Level_getEnemies(Level *lvl);
extern "C" void KIPlayer_ResumeEngineSound(KIPlayer *k);

// MGame::resumeSounds(): resume FMOD, the player's engine sound, and every enemy's.
extern "C" void MGame_resumeSounds(MGame *self) {
    FModSound_resumeAll(*g_fmod);
    PlayerEgo_ResumeEngineSound(F<PlayerEgo *>(self, 0x58));
    EnemyList *e = Level_getEnemies(F<Level *>(self, 0x78));
    if (e == 0) return;
    for (uint32_t i = 0; i < e->size; i++)
        KIPlayer_ResumeEngineSound(e->data[i]);
}
