#include "class.h"

extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py);
extern "C" void FModSound_play(void *snd, int id, void *a, void *b, float f);

// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

extern "C" unsigned int TouchButton_OnTouchEnd(TouchButton *self, int px, int py)
{
    unsigned int res;
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0) {
        res = 0;
    } else if (UC(self, 0xb0) == 0 || TouchButton_touchedInside(self, px, py) == 0) {
        res = 0;
        UC(self, 0xb0) = 0;
    } else {
        UC(self, 0xb0) = 0;
        FModSound_play(*g_TB_sound, 0x7b, 0, 0, 0.0f);
        res = 1;
    }
    return res;
}
