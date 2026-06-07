#include "class.h"

// FModSound::promptMusicCue(int)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x28](music, p1)
struct FModSound { void promptMusicCue(int p1); };
void FModSound::promptMusicCue(int p1)
{
    void *music = pp(this, OFF_MUSIC);
    if (!music)
        return;
    void **vt = *(void ***)music;
    typedef void (*Fn)(void *, int);
    return ((Fn)vt[0x28 / 4])(music, p1);
}
