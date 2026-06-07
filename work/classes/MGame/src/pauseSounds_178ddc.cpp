#include "class.h"

struct FModSound;
struct PlayerEgo;
struct KIPlayer;
struct Level;
struct EnemyList { uint32_t size; KIPlayer **data; };

__attribute__((visibility("hidden"))) extern FModSound **g_fmod;
extern "C" void FModSound_pauseAllPlayingSoundFXEvents(FModSound *s);
extern "C" void PlayerEgo_PauseEngineSound(PlayerEgo *p);
extern "C" EnemyList *Level_getEnemies(Level *lvl);
extern "C" void KIPlayer_PauseEngineSound(KIPlayer *k);

// MGame::pauseSounds(): snapshot the pause flag, pause FMOD, the player's engine sound,
// and every enemy's.
extern "C" void MGame_pauseSounds(MGame *self) {
    F<uint8_t>(self, 0x1a6) = F<uint8_t>(self, 0x5d);
    FModSound_pauseAllPlayingSoundFXEvents(*g_fmod);
    PlayerEgo_PauseEngineSound(F<PlayerEgo *>(self, 0x58));
    EnemyList *e = Level_getEnemies(F<Level *>(self, 0x78));
    if (e != 0) {
        for (uint32_t i = 0; i < e->size; i++)
            KIPlayer_PauseEngineSound(e->data[i]);
    }
}
