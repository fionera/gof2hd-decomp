#include "class.h"

extern "C" void FModSound_play(void *sound, int id, void *pos, void *vel, float volume);
__attribute__((visibility("hidden"))) extern void **g_HackingGame_sound_left;

void HackingGame::rotateLeftCW(bool sound)
{
    if (I(this, 0x130) != 0)
        return;
    if (sound)
        FModSound_play(*g_HackingGame_sound_left, 0x8e2, 0, 0, 0.0f);
    return rotateLeftCW((int *)((char *)this + 0x34));
}
