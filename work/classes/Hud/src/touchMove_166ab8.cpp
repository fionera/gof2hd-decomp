#include "class.h"

extern "C" unsigned int Hud_touchMoveFallback(Hud *self, unsigned int a, void *b);
extern "C" float Globals_sqrt(void *g, float v);
extern "C" int __aeabi_idiv(int a, int b);
// Globals singleton (cell value = holder; object = *holder).
__attribute__((visibility("hidden"))) extern void **g_Hud_globals;

extern "C" unsigned int Hud_touchMove(Hud *self, unsigned int a, void *b, int key)
{
    int arr = I((void *)I(self, 0x28c), 4);
    unsigned int i = 0;
    for (; i <= 0x18; i = i + 1) {
        if (I((void *)arr, i * 4) == key && I((void *)I(self, 0x290), i * 4) == 0x20)
            goto found;
    }
    return Hud_touchMoveFallback(self, a, b);
found:
    int dx = (int)a - (int)US(self, 0x424);
    int dy = (int)(unsigned int)b - (int)US(self, 0x426);
    float f = (float)(dy * dy + dx * dx);
    float r = Globals_sqrt(*g_Hud_globals, f);
    int denom = I(self, 0x4e0);
    int len = (int)r;
    if (denom < len) {
        short s = __aeabi_idiv(denom * dx, len);
        short base = S(self, 0x426);
        S(self, 0x41e) = s + S(self, 0x424);
        s = __aeabi_idiv(denom * dy, len);
        S(self, 0x420) = s + base;
    } else {
        S(self, 0x41e) = (short)a;
        S(self, 0x420) = (short)(unsigned int)b;
    }
    return 0x20;
}
