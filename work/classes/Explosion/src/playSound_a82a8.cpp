#include "class.h"

extern int Explosion_soundDefault;
extern int Explosion_soundFallback;
extern int Explosion_soundSpecial;
extern char Explosion_soundSettings[];
extern void *Explosion_random;

extern "C" int AERandom_nextInt(void *self, int bound);
extern "C" void FModSound_play(int sound, int id, Vector *pos, int zero, int zero2);

extern "C" void Explosion_playSound(Explosion *self, Vector *pos)
{
    Vector *soundPos = pos;
    int soundId = I(self, 0x28);
    int cue;
    int sound;
    int enabled;

    if (soundId >= 0) {
        uint32_t offset = (uint32_t)(soundId - 0x29);
        if (offset <= 0x15) {
            cue = 0xf;
            switch (soundId) {
            case 0x29:
                break;
            case 0x2a:
                cue = 0x10;
                break;
            case 0x2b:
                cue = 0x11;
                break;
            case 0x2c:
                cue = 0xe;
                break;
            case 0x2d:
                cue = 0xd;
                break;
            case 0x2e:
                cue = 0xc;
                break;
            case 0x2f:
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3a:
            case 0x3b:
                return;
            case 0x3c:
            case 0x3d:
            case 0x3e:
                cue = 0x16;
                break;
            }
        } else {
            offset = (uint32_t)(soundId - 0xb0);
            if (offset < 3) {
                cue = 0x16;
            } else if (soundId == 0xb3) {
                cue = 0xc;
            } else if (soundId == 0xc5 || soundId == 0xdd) {
                cue = 0x8cd;
            } else {
                if (soundId != 0xe8) {
                    return;
                }
                cue = 0x8e7;
            }
        }
        sound = Explosion_soundDefault;
        enabled = Explosion_soundSettings[0xf];
    } else {
        int type = I(self, 0x0);
        uint32_t typeOffset = (uint32_t)(type - 2);
        if (typeOffset < 4) {
            sound = Explosion_soundSpecial;
            enabled = Explosion_soundSettings[0xf];
            if (enabled == 0) {
                soundPos = 0;
            }
            FModSound_play(sound, 0x15, soundPos, 0, 0);
            return;
        }
        if (type != 0) {
            return;
        }
        sound = Explosion_soundFallback;
        int random = AERandom_nextInt(Explosion_random, 2);
        enabled = Explosion_soundSettings[0xf];
        cue = 0x13;
        if (random == 0) {
            cue = 0x12;
        }
    }

    if (enabled == 0) {
        soundPos = 0;
    }
    FModSound_play(sound, cue, soundPos, 0, 0);
}
