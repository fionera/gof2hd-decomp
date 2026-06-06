#include "class.h"

extern "C" int gStopSoundIds[];
extern "C" void *gFModSound;
extern "C" void *gFModSoundAlt;
extern "C" void Player_stopShootSound_play_tail(void *sound, int id);

__attribute__((minsize)) extern "C" void Player_stopShootSound(Player *self, int index, int channel)
{
    if ((unsigned int)channel > 8) {
        return;
    }
    if (((1 << channel) & 0x10c) != 0) {
        void *sound;
        int id;
        if (self->kiPlayer != 0 && *(int *)((char *)self->kiPlayer + 0x28) == 9) {
            id = 0x3e;
            sound = gFModSoundAlt;
        } else {
            id = gStopSoundIds[index];
            sound = gFModSound;
        }
        return Player_stopShootSound_play_tail(sound, id);
    }
}
