#include "class.h"

extern "C" void ScrollTouchWindow_update(void *self, int dt);
extern "C" void ChoiceWindow_update(void *self, int dt);
extern "C" void FModSound_getPlayingProgress(void *self, int sound);
extern "C" int FModSound_isPlaying(void *self, int sound);
extern "C" int DialogueWindow_isLastPage(DialogueWindow *self);
extern "C" int DialogueWindow_nextPage(DialogueWindow *self);

__attribute__((visibility("hidden"))) extern void *g_dw_soundConfig;
__attribute__((visibility("hidden"))) extern void **g_dw_fmodSound;

extern "C" void DialogueWindow_update(DialogueWindow *self, int dt)
{
    void *scroll = F<void *>(self, 0x40);
    if (scroll != 0) {
        ScrollTouchWindow_update(scroll, dt);
    }
    if (F<uint8_t>(self, 0x54) != 0) {
        ChoiceWindow_update(F<void *>(self, 0x50), dt);
    }
    if (F<uint8_t>(g_dw_soundConfig, 0xe) != 0 && F<int>(self, 0x64) != -1) {
        void **sound = g_dw_fmodSound;
        FModSound_getPlayingProgress(*sound, F<int>(self, 0x64));
        void *playingSound = *sound;
        int playingId = F<int>(self, 0x64);
        if (FModSound_isPlaying(playingSound, playingId) == 0 &&
            DialogueWindow_isLastPage(self) == 0) {
            int elapsed = F<int>(self, 0x68);
            if (elapsed >= F<int>(self, 0x6c)) {
                DialogueWindow_nextPage(self);
                elapsed = F<int>(self, 0x68);
            }
            F<int>(self, 0x68) = elapsed + dt;
        }
    }
}
