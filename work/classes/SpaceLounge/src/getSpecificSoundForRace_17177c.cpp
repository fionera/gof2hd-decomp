#include "class.h"

extern "C" int SpaceLounge_getSpecificSoundForRace(SpaceLounge *, unsigned soundId, int race, bool alternate)
{
    unsigned delta;

    switch (race) {
    case 0:
    case 5:
        delta = soundId - 0x2f7;
        if (alternate) {
            if ((unsigned)delta < 0x30) {
                return soundId + 0xf0;
            }
        } else if ((unsigned)delta < 0x30) {
            return soundId + 0xc0;
        }
        break;
    case 1:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x120;
        }
        break;
    case 2:
    case 3:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x90;
        }
        break;
    case 4:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x60;
        }
        break;
    case 6:
        goto done;
    case 7:
        delta = soundId - 0x2f7;
        if ((unsigned)delta < 0x30) {
            return soundId + 0x30;
        }
        break;
    }

    soundId = -1;
done:
    return soundId;
}
