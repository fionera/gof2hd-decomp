#include "class.h"

// Hud::hudEventMedal(int medalId, int percent) — composes the "<medal name> NN%" event line
// (clamping percent to 100), and if it differs from the last shown event, pushes it onto the
// event queue and sets up its scroll/centering state.
extern "C" void *GameText_getText(void *gt, int id);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void String_ctor_copy(void *s, void *src, bool b);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_dtor(void *s);
extern "C" int  Hud_sameHudEventAsBefore(Hud *self, void *str);
extern "C" void *operator_new(uint32_t);
extern "C" void ListItem_ctor(void *li, void *str, int kind);
extern "C" void Hud_addToEventQueue(Hud *self, void *item);
extern "C" int  PaintCanvas_GetTextWidth(void *font, void *str);

__attribute__((visibility("hidden"))) extern void **g_Hud_gameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_meCanvas; // font holder
__attribute__((visibility("hidden"))) extern void **g_Hud_meFont;   // string holder
__attribute__((visibility("hidden"))) extern void **g_Hud_meScreenW;// [0]=screen width
extern const char g_Hud_meSep[] __attribute__((visibility("hidden")));
extern const char g_Hud_meEnd[] __attribute__((visibility("hidden")));

extern "C" void Hud_hudEventMedal(Hud *self, int medalId, int percent)
{
    void *gt = *g_Hud_gameText;
    void *name = GameText_getText(gt, medalId + 0x5e3);

    char sep[12], acc1[12], num[12], acc2[12], end[12], acc3[12];
    String_ctor_cstr(sep, g_Hud_meSep, false);
    String_concat(acc1, name, sep);
    if (percent >= 100) percent = 100;
    String_ctor_int(num, percent);
    String_concat(acc2, acc1, num);
    String_ctor_cstr(end, g_Hud_meEnd, false);
    String_concat(acc3, acc2, end);

    void *dst = B(self, 0x1e0);
    String_op_assign(dst, acc3);
    String_dtor(acc3);
    String_dtor(end);
    String_dtor(acc2);
    String_dtor(num);
    String_dtor(acc1);
    String_dtor(sep);

    char probe[12];
    String_ctor_copy(probe, dst, false);
    int same = Hud_sameHudEventAsBefore(self, probe);
    String_dtor(probe);
    if (same != 0) return;

    void *item = operator_new(0x48);
    void *str = operator_new(0xc);
    String_ctor_copy(str, dst, false);
    ListItem_ctor(item, str, 3);
    Hud_addToEventQueue(self, item);

    int w = PaintCanvas_GetTextWidth(*g_Hud_meCanvas, *g_Hud_meFont);
    int screenW = *(int *)*g_Hud_meScreenW;
    I(self, 0x1d8) = 0;
    UC(self, 0x1de) = 1;
    UC(self, 0x1ec) = ((screenW / 2 - I(self, 0x4e8)) + I(self, 0x4f0) * -2 < w) ? 1 : 0;
}
