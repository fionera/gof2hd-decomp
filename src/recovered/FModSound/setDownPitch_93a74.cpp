#include "class.h"

// FModSound::setDownPitch(bool)
//   pitch = down ? -1.0 : 0.1 ; this[8]=down ; for i<0x8f5: if(system&&events[i]&&isPlaying(i)) setPitch
extern "C" void FMOD_Event_setPitch(void *event, float pitch, int mode);
static const float kUp = 0.1f;

struct FModSound {
    unsigned isPlaying(int p1);
    void setDownPitch(bool down);
};
void FModSound::setDownPitch(bool down)
{
    float pitch = down ? -1.0f : kUp;
    u8(this, 8) = (uint8_t)down;
    for (unsigned i = 0; i < 0x8f5u; ++i) {
        if (u32(this, OFF_SYSTEM) && pp(this, i * 4 + OFF_EVENTS) && isPlaying(i))
            FMOD_Event_setPitch(pp(this, i * 4 + OFF_EVENTS), pitch, 1);
    }
}
