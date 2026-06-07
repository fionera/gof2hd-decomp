#include "class.h"

struct Music;
struct Cfg;
// Singletons held as pointer-to-pointer in hidden globals (single PC-relative deref).
__attribute__((visibility("hidden"))) extern Music **g_music;
__attribute__((visibility("hidden"))) extern Cfg **g_cfg;
extern "C" int FModSound_tryToStopMusicForBGMusic();
extern "C" void Music_resume(Music *m, int one, int v);

// MGame::OnResume(): resume background music unless it was deliberately stopped.
extern "C" void MGame_OnResume() {
    Music **mp = g_music;
    if (*mp == 0) return;
    if (FModSound_tryToStopMusicForBGMusic() != 0) return;
    return Music_resume(*mp, 1, **(int **)g_cfg);
}
