#include "class.h"

// FModSound::setMusicParamValue(int, float)
//   music = this->0x2400; if (music) tail-call music->vtbl[0x34](music, p1, p2)
struct FModSound { void setMusicParamValue(int p1, float p2); };
void FModSound::setMusicParamValue(int p1, float p2)
{
    void *music = pp(this, OFF_MUSIC);
    if (!music)
        return;
    void **vt = *(void ***)music;
    typedef void (*Fn)(void *, int, float);
    return ((Fn)vt[0x34 / 4])(music, p1, p2);
}
