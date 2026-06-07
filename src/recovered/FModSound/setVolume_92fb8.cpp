#include "class.h"

// FModSound::setVolume(int, float)
//   if (system && (h=this[p1*4+0x23ec])) tail-call h->vtbl[0x20](h, vol)
struct FModSound { void setVolume(int p1, float vol); };
void FModSound::setVolume(int p1, float vol)
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *h = pp(this, p1 * 4 + OFF_CATEGORY);
    if (!h)
        return;
    void **vt = *(void ***)h;
    typedef void (*Fn)(void *, float);
    return ((Fn)vt[0x20 / 4])(h, vol);
}
