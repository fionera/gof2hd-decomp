#include "class.h"

// FModSound::enableCategory(int p1, bool enable)
extern "C" void FMOD_play(void *self, int a, void *b, float v);

struct FModSound { void enableCategory(int p1, bool enable); };
void FModSound::enableCategory(int p1, bool enable)
{
    if (p1 > 3)
        return;
    if (u32(this, OFF_SYSTEM) == 0)
        return;
    void *cat = pp(this, p1 * 4 + OFF_CATEGORY);
    if (cat == 0)
        return;

    u8(this, p1 + OFF_ENABLED) = (uint8_t)enable;
    if (!enable) {
        void **vt = *(void ***)cat;
        typedef void (*Fn)(void *);
        ((Fn)vt[0x1c / 4])(cat);
    } else if (p1 == 1 && i32(this, 0) >= 0) {
        FMOD_play(this, i32(this, 0), 0, 0.0f);
    }

    uint8_t any = 0;
    int i = 0x12;
    do {
        if ((unsigned)(i - 0x11) > 3)
            break;
        uint8_t v = u8(this, i);
        i++;
        any = (any || v) ? 1 : 0;
    } while (u8(this, i - 1) == 0);
    u8(this, OFF_ENABLED) = any;
}
