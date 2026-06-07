#include "class.h"

namespace FMOD { struct EventCategory; }

// FModSound::pauseAllPlayingSoundFXEvents()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) { getCategory(h,&cat);
//       cat->vtbl[0](cat,&a,&b); if (a==1) setPaused(h,1); }
extern "C" int FMOD_Event_getCategory(void *event, FMOD::EventCategory **out);
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound {
    unsigned isPlaying(int p1);
    void pauseAllPlayingSoundFXEvents();
};
void FModSound::pauseAllPlayingSoundFXEvents()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM)) {
            void *h = pp(this, i * 4 + OFF_EVENTS);
            if (h && isPlaying(i)) {
                FMOD::EventCategory *cat;
                FMOD_Event_getCategory(h, &cat);
                int a, b;
                void **vt = *(void ***)cat;
                typedef void (*Fn)(void *, int *, int *);
                ((Fn)vt[0])(cat, &a, &b);
                if (a == 1)
                    FMOD_Event_setPaused(pp(this, i * 4 + OFF_EVENTS), 1);
            }
        }
    }
}
