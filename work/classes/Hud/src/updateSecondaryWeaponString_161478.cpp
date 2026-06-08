#include "class.h"

// Hud::updateSecondaryWeaponString() — rebuilds the "<weapon name> xNN" label shown next to
// the secondary-fire button (field +0x3b4) and recomputes its centered X position (+0x3c0).
extern "C" void *GameText_getText(void *gt, int id);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void String_concat(void *out, void *lhs, void *rhs);     // operator+
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_dtor(void *s);
extern "C" int  Item_getIndex(void *item);
extern "C" int  Item_getAmount(void *item);
extern "C" int  PaintCanvas_GetTextWidth(void *font, void *str);

__attribute__((visibility("hidden"))) extern void **g_Hud_gameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_swCanvas;  // font holder
__attribute__((visibility("hidden"))) extern void **g_Hud_swFont;    // string holder
__attribute__((visibility("hidden"))) extern void **g_Hud_swScreenW; // [0]=screen width
extern const char g_Hud_swSep[]  __attribute__((visibility("hidden"))); // " x" separator
extern const char g_Hud_swEnd[]  __attribute__((visibility("hidden"))); // trailing string

extern "C" void Hud_updateSecondaryWeaponString(Hud *self)
{
    void *item = P(self, 0x258);
    if (item == 0) return;

    void *gt = *g_Hud_gameText;
    int idx = Item_getIndex(item);
    void *name = GameText_getText(gt, idx + 0x4fa);

    char sep[12], acc1[12], amount[12], acc2[12], end[12], acc3[12];
    String_ctor_cstr(sep, g_Hud_swSep, false);
    String_concat(acc1, name, sep);
    int amt = Item_getAmount(item);
    String_ctor_int(amount, amt);
    String_concat(acc2, acc1, amount);
    String_ctor_cstr(end, g_Hud_swEnd, false);
    String_concat(acc3, acc2, end);

    String_op_assign(B(self, 0x3b4), acc3);
    String_dtor(acc3);
    String_dtor(end);
    String_dtor(acc2);
    String_dtor(amount);
    String_dtor(acc1);
    String_dtor(sep);

    int screenW = *(int *)*g_Hud_swScreenW;
    int w = PaintCanvas_GetTextWidth(*g_Hud_swCanvas, *g_Hud_swFont);
    I(self, 0x3c0) = (screenW >> 1) - (w >> 1);
}
