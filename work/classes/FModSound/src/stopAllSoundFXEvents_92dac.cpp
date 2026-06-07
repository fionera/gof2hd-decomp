#include "class.h"

// FModSound::stopAllSoundFXEvents()
//   if (system) iterate category indices (inner skip on ==1, stop at 4): cat[i]->vtbl[0x1c](cat[i])
struct FModSound { void stopAllSoundFXEvents(); };
void FModSound::stopAllSoundFXEvents()
{
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void **cats = (void **)((char *)this + 0x23f0);
    unsigned j = 0;
    while (true) {
        unsigned i;
        do {
            i = j;
            j = i + 1;
        } while ((j & 0x7fffffff) == 1);
        if ((j & 0x7fffffff) == 4)
            break;
        void *c = cats[i];
        void **vt = *(void ***)c;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(c);
    }
}
