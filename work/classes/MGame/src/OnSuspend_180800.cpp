#include "class.h"

struct RecordHandler;
struct MenuTouchWindow;
struct FModSound;
struct PlayerEgo;
struct KIPlayer;
struct Level;
struct EnemyList { uint32_t size; KIPlayer **data; };

__attribute__((visibility("hidden"))) extern RecordHandler **g_record;
__attribute__((visibility("hidden"))) extern FModSound **g_fmod;
extern "C" void RecordHandler_saveOptions(RecordHandler *r);
extern "C" void MGame_pauseSounds(MGame *self);
extern "C" void *MGame_opnew(unsigned sz);
extern "C" void MenuTouchWindow_ctor(MenuTouchWindow *w, int v);
extern "C" void FModSound_pauseAllPlaying(FModSound *s);
extern "C" void PlayerEgo_PauseEngineSound(PlayerEgo *p);
extern "C" EnemyList *Level_getEnemies(Level *lvl);
extern "C" void KIPlayer_PauseEngineSound(KIPlayer *k);
extern "C" int PlayerEgo_isDead(PlayerEgo *p);
extern "C" void MenuTouchWindow_setCutsceneMode(MenuTouchWindow *w, int mode);
extern "C" void Level_onSuspend(Level *lvl);  // tail call via this->[0x74]

// MGame::OnSuspend(): persist options, pause all audio, raise the pause dialogue (cutscene
// mode if the player is mid-cutscene/dead), then suspend the level.
extern "C" void MGame_OnSuspend(MGame *self) {
    if (*g_record != 0) RecordHandler_saveOptions(*g_record);
    MGame_pauseSounds(self);
    if (F<uint8_t>(self, 0x5d) == 0) {
        if (F<MenuTouchWindow *>(self, 0x88) == 0) {
            MenuTouchWindow *w = (MenuTouchWindow *)MGame_opnew(0x240);
            MenuTouchWindow_ctor(w, 1);
            F<MenuTouchWindow *>(self, 0x88) = w;
        }
        F<uint8_t>(self, 0x1a6) = 1;
        F<uint8_t>(self, 0x5d) = 1;
        FModSound_pauseAllPlaying(*g_fmod);
        PlayerEgo_PauseEngineSound(F<PlayerEgo *>(self, 0x58));
        EnemyList *e = Level_getEnemies(F<Level *>(self, 0x78));
        if (e != 0) {
            for (uint32_t i = 0; i < e->size; i++)
                KIPlayer_PauseEngineSound(e->data[i]);
        }
        MenuTouchWindow *w = F<MenuTouchWindow *>(self, 0x88);
        int mode = 1;
        if (F<uint8_t>(self, 0x5f) == 0)
            mode = PlayerEgo_isDead(F<PlayerEgo *>(self, 0x58));
        MenuTouchWindow_setCutsceneMode(w, mode);
        F<uint8_t>(self, 0xc9) = 1;
    }
    return Level_onSuspend(F<Level *>(self, 0x74));
}
