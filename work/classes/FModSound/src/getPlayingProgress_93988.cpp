#include "class.h"

extern "C" {
int FMOD_Event_getInfo(void *event, char **name, void *info);
float VectorSignedToFloat(int v, int mode);
}

// FModSound::getPlayingProgress(int) -> queries the FMOD event info for a slot
// and converts two of its fields to float (progress numerator/denominator).
struct FModSound {
    float getPlayingProgress(int idx);
};

float FModSound::getPlayingProgress(int idx)
{
    if (pp(this, OFF_FLAG2404) != 0 && pp(this, OFF_SYSTEM) != 0 &&
        u8(this, OFF_ENABLED) != 0 &&
        pp(this, idx * 4 + OFF_EVENTS) != 0) {
        char *name = 0;
        int info[8];
        FMOD_Event_getInfo(pp(this, idx * 4 + OFF_EVENTS), &name, info);
        VectorSignedToFloat(info[2], 0);
        VectorSignedToFloat(info[1], 0);
    }
    return 0.0f;
}
