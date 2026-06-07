#include "class.h"

// FModSound::fadeOutNow()
//   s = this[0]; if (s==-1 || this[4]==s) return; tail-call ext(this, 0, s, kConst)
extern "C" void FMOD_fade(void *self, int a, int s, float v);
static const float kFade = 0.1f;

struct FModSound { void fadeOutNow(); };
void FModSound::fadeOutNow()
{
    int s = i32(this, 0);
    if (s == -1)
        return;
    if (i32(this, 4) == s)
        return;
    return FMOD_fade(this, 0, s, kFade);
}
