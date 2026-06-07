#include "class.h"

extern "C" void *Array_ImagePart_dtor(void *arr);
extern "C" void *ScrollTouchWindow_dtor(void *window);
extern "C" void *ImageFactory_loadChar(void *factory, void *parts);
extern "C" void *Wanted_getImageParts(void *wanted);
extern "C" void Wanted_getName(String *out, void *wanted);
extern "C" int Wanted_isActive(void *wanted);
extern "C" int Wanted_isTerminated(void *wanted);
extern "C" int Wanted_getLastSeen(void *wanted);
extern "C" int Wanted_getTravelsTo(void *wanted);
extern "C" int Wanted_getCurrentLocation(void *wanted);
extern "C" int Wanted_getReward(void *wanted);
extern "C" int Wanted_getIndex(void *wanted);
extern "C" void *Galaxy_getStation(void *galaxy, int id);
extern "C" int Station_getSystem(void *station);
extern "C" void Station_getName(String *out, void *station);
extern "C" void SolarSystem_getName(String *out, int system);
extern "C" void *Station_dtor(void *station);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void String_cstr_ctor(String *s, const char *text, bool copy);
extern "C" void String_copy_ctor(String *s, String *src, bool copy);
extern "C" void String_int_ctor(String *s, int value);
extern "C" void String_assign(String *dst, String *src);
extern "C" void String_plus(String *out, String *a, String *b);
extern "C" void String_plusAssign(String *dst, String *src);
extern "C" void String_dtor(String *s);
extern "C" void ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool flag);
extern "C" void ScrollTouchWindow_setText(void *self, String *a, String *b);

__attribute__((visibility("hidden"))) extern int *g_WantedWindow_select_stack;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_factory;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_galaxy;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_text_a;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_select_layout;
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_terminated_d[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_from[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_to[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_at[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_reward[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_space[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_a[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_b[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_c[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_line_d[];
__attribute__((visibility("hidden"))) extern const char g_WantedWindow_s_empty[];

static inline void *wanted_at(WantedWindow *self, int idx)
{
    return ArrayItem(F<void *>(self, 0x38), idx);
}

static inline void append_label(String *dst, const char *prefix, String *value)
{
    String a;
    String b;
    String_cstr_ctor(&a, prefix, false);
    String_plus(&b, &a, value);
    String_plusAssign(dst, &b);
    String_dtor(&b);
    String_dtor(&a);
}

extern "C" void WantedWindow_selectWanted(WantedWindow *self, int idx)
{
    String s34;
    String s40;
    String s4c;
    String s58;
    String s64;
    String s70;
    String s7c;
    String s88;
    String s94;

    if (F<void *>(self, 0x8) != 0) {
        operator_delete(Array_ImagePart_dtor(F<void *>(self, 0x8)));
    }
    F<void *>(self, 0x8) = 0;

    if (F<void *>(self, 0x2c) != 0) {
        operator_delete(ScrollTouchWindow_dtor(F<void *>(self, 0x2c)));
    }
    F<void *>(self, 0x2c) = 0;

    void *wanted = wanted_at(self, idx);
    F<void *>(self, 0x8) = ImageFactory_loadChar(*g_WantedWindow_select_factory,
                                                  Wanted_getImageParts(wanted));

    Wanted_getName(&s34, wanted);
    String_assign((String *)((char *)self + 0x54), &s34);
    String_dtor(&s34);

    if (Wanted_isActive(wanted) != 0) {
        void *galaxy = *g_WantedWindow_select_galaxy;
        void *last = Galaxy_getStation(galaxy, Wanted_getLastSeen(wanted));
        void *travel = Galaxy_getStation(galaxy, Wanted_getTravelsTo(wanted));
        void *current = Galaxy_getStation(galaxy, Wanted_getCurrentLocation(wanted));

        Station_getName(&s58, last);
        String_cstr_ctor(&s64, g_WantedWindow_s_from, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(last));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x3c), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        Station_getName(&s58, travel);
        String_cstr_ctor(&s64, g_WantedWindow_s_to, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(travel));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x48), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        Station_getName(&s58, current);
        String_cstr_ctor(&s64, g_WantedWindow_s_at, false);
        String_plus(&s4c, &s58, &s64);
        SolarSystem_getName(&s70, Station_getSystem(current));
        String_plus(&s40, &s4c, &s70);
        String_cstr_ctor(&s7c, g_WantedWindow_s_space, false);
        String_plus(&s34, &s40, &s7c);
        String_assign((String *)((char *)self + 0x6c), &s34);
        String_dtor(&s34);
        String_dtor(&s7c);
        String_dtor(&s40);
        String_dtor(&s70);
        String_dtor(&s4c);
        String_dtor(&s64);
        String_dtor(&s58);

        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
        String_dtor(&s4c);
        String_dtor(&s40);

        if (last != 0) {
            operator_delete(Station_dtor(last));
        }
        if (travel != 0) {
            operator_delete(Station_dtor(travel));
        }
        if (current != 0) {
            operator_delete(Station_dtor(current));
        }
    } else if (Wanted_isTerminated(wanted) != 0) {
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_a, false);
        String_assign((String *)((char *)self + 0x3c), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_b, false);
        String_assign((String *)((char *)self + 0x48), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_c, false);
        String_assign((String *)((char *)self + 0x6c), &s34);
        String_dtor(&s34);
        String_cstr_ctor(&s34, g_WantedWindow_s_terminated_d, false);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
    } else {
        String *text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x3c), text);
        text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x48), text);
        String_int_ctor(&s40, Wanted_getReward(wanted));
        String_cstr_ctor(&s4c, g_WantedWindow_s_reward, false);
        String_plus(&s34, &s40, &s4c);
        String_assign((String *)((char *)self + 0x78), &s34);
        String_dtor(&s34);
        String_dtor(&s4c);
        String_dtor(&s40);
        text = GameText_getText(*g_WantedWindow_select_text_a, 0xc9d);
        String_assign((String *)((char *)self + 0x6c), text);
    }

    Wanted_isTerminated(wanted);
    String_assign((String *)((char *)self + 0x60),
                  GameText_getText(*g_WantedWindow_select_text_a, 0xc9d));

    void *layout = *g_WantedWindow_select_layout;
    int y = F<int>(self, 0x20);
    int h = F<int>(self, 0x28);
    int top = y + F<int>(layout, 0x0c) + F<int>(layout, 0x20) +
              F<int>(layout, 0x5c) + F<int>(layout, 0x2c);
    int height = (((((y - F<int>(layout, 0x2c)) + h) - top) -
                   F<int>(layout, 0x10)) - F<int>(layout, 0x2d8)) -
                 F<int>(layout, 0x24);
    if (Wanted_isActive(wanted) != 0) {
        height = (height - F<int>(layout, 0x4c)) - F<int>(layout, 0x30);
    }

    void *scroll = operator_new(0x24);
    int pad = F<int>(layout, 0x2c);
    ScrollTouchWindow_ctor(scroll, F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1) + pad,
                           F<int>(layout, 0x2d8) + pad + top,
                           ((F<int>(self, 0x24) >> 1) - pad) - F<int>(layout, 0x28),
                           height, false);
    F<void *>(self, 0x2c) = scroll;

    String_cstr_ctor(&s34, g_WantedWindow_s_empty, false);
    String_assign(&s34, GameText_getText(*g_WantedWindow_select_text_a, 0xc9d));
    append_label(&s34, g_WantedWindow_s_line_a,
                 (String *)((char *)self + 0x3c));
    append_label(&s34, g_WantedWindow_s_line_b,
                 (String *)((char *)self + 0x48));
    append_label(&s34, g_WantedWindow_s_line_c,
                 (String *)((char *)self + 0x6c));
    Wanted_getIndex(wanted);
    append_label(&s34, g_WantedWindow_s_line_d,
                 (String *)((char *)self + 0x78));

    String_cstr_ctor(&s88, g_WantedWindow_s_empty, false);
    String_copy_ctor(&s94, &s34, false);
    ScrollTouchWindow_setText(F<void *>(self, 0x2c), &s88, &s94);
    String_dtor(&s94);
    String_dtor(&s88);
    String_dtor(&s34);
}
