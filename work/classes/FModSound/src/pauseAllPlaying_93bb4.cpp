#include "class.h"

// FModSound::pauseAllPlaying()
//   for i<0x8f5: if (system && events[i] && isPlaying(i)) Event::setPaused(events[i],1)
extern "C" int FMOD_Event_setPaused(void *event, int paused);

struct FModSound {
    unsigned isPlaying(int p1);
    void pauseAllPlaying();
};
void FModSound::pauseAllPlaying()
{
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM)) {
            void **slot = (void **)((char *)this + i * 4 + OFF_EVENTS);
            if (*slot && isPlaying(i))
                FMOD_Event_setPaused(*slot, 1);
        }
    }
}
