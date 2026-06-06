#include "class.h"

extern "C" int gShootSoundsByType[];
extern "C" int gShootSoundsByIndex[];
extern "C" void **gFModSoundPtr;
extern "C" void *gAppManagerA;
extern "C" void *gAppManagerB;
extern "C" void *gAppManagerC;
extern "C" int FModSound_isPlaying(void *sound, int id);
extern "C" void FModSound_updateEvent3DAttributes(void *sound, int id, Vector *pos, Vector *vel, bool b);
extern "C" void Player_playShootSound_play_tail(float vol, void *sound, int id, Vector *pos, int z);

__attribute__((minsize)) extern "C" void Player_playShootSound(Player *self, int type, Vector *channel, float volume)
{
    int soundId;
    if (self->kiPlayer == 0) {
        soundId = gShootSoundsByIndex[type];
    } else {
        unsigned int kind = *(unsigned int *)((char *)self->kiPlayer + 0x28);
        if (kind < 0xb) {
            soundId = gShootSoundsByType[kind];
        } else {
            soundId = 0x3d;
        }
    }
    void **fmodPtr = gFModSoundPtr;
    void *sound = fmodPtr[0];
    Vector *pos = channel;
    if ((unsigned int)channel < 9 && ((1 << ((unsigned int)channel & 0xff)) & 0x10c) != 0) {
        if (FModSound_isPlaying(sound, soundId) != 0) {
            if (*((char *)gAppManagerA + 0xf) != 0) {
                FModSound_updateEvent3DAttributes(fmodPtr[0], soundId, pos, 0, false);
            }
            return;
        }
        if (*((char *)gAppManagerB + 0xf) == 0) {
            pos = 0;
        }
        sound = fmodPtr[0];
    } else if (*((char *)gAppManagerC + 0xf) == 0) {
        pos = 0;
    }
    return Player_playShootSound_play_tail(volume, sound, soundId, pos, 0);
}
