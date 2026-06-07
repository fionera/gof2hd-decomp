#include "class.h"

extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py);
extern "C" void FModSound_play(void *snd, int id, void *a, void *b, float f);

// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

extern "C" bool TouchButton_OnTouchBegin(TouchButton *self, int px, int py)
{
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0)
        return false;
    int r = TouchButton_touchedInside(self, px, py);
    UC(self, 0xb0) = (unsigned char)r;
    if (r == 0)
        return false;
    FModSound_play(*g_TB_sound, 0x7c, 0, 0, 0.0f);
    return UC(self, 0xb0) != 0;
}
