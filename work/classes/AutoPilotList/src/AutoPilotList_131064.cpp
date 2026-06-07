#include "class.h"

namespace AbyssEngine { struct String; }
using AbyssEngine::String;

extern "C" String *GameText_getText(void *self, int id);                  // 0x72f70
extern "C" void Array_String_ctor(void *arr);                            // 0x6f628
extern "C" void ArraySetLength_String(int n, void *arr);                 // 0x6fe08
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);     // 0x6ee18
extern "C" void String_ctor_copy(void *out, const String *src, bool);    // 0x6f028
extern "C" void String_plus(void *out, const void *a, const void *b);    // 0x6ef98 operator+
extern "C" void Station_getName(void *out, void *station);               // 0x736a8
extern "C" void *Status_getSystem(void *status);                         // 0x71a10
extern "C" int SolarSystem_currentOrbitHasWarpGate(void *system);        // 0x73c18
extern "C" int Status_inEmptyOrbit(void *status);                        // 0x73d20
extern "C" void *Status_getStation(void *status);                        // 0x71c14
extern "C" void *Level_getPlayer(void *level);                           // 0x72034
extern "C" void *PlayerEgo_getRoute(void *player);                       // 0x768b8
extern "C" void *Route_getLastWaypoint(void *route);                     // 0x71ee4
extern "C" int PaintCanvas_GetTextWidth(void *canvas, const String *s);  // 0x6faa8
extern "C" void _ZN13AutoPilotList4downEv(AutoPilotList *self);          // down()

typedef void (*StringDtorFn)(void *self);

__attribute__((visibility("hidden"))) extern int **g_APL_apFlag;         // status-active flag
__attribute__((visibility("hidden"))) extern void **g_APL_gametext;      // GameText singleton
__attribute__((visibility("hidden"))) extern void **g_APL_status;        // Status singleton
__attribute__((visibility("hidden"))) extern StringDtorFn g_APL_strDtor;
__attribute__((visibility("hidden"))) extern void **g_APL_font;          // measurement font String*
__attribute__((visibility("hidden"))) extern void **g_APL_canvas;        // PaintCanvas
__attribute__((visibility("hidden"))) extern int **g_APL_screenW;        // screen width
__attribute__((visibility("hidden"))) extern int **g_APL_screenH;        // screen height
extern const char kApLit1[] __attribute__((visibility("hidden")));
extern const char kApLit2[] __attribute__((visibility("hidden")));

static inline void **entryData(AutoPilotList *self) {
    return *(void ***)((char *)F<void *>(self, 0x10) + 0x4);
}

// AutoPilotList::AutoPilotList(Level*) - build the list of autopilot destinations (current
// station, warp gate, mission target, "cancel", route waypoint), measure the widest entry
// to size the window, center it, then select the first non-empty row.
extern "C" void _ZN13AutoPilotListC1EP5Level(AutoPilotList *self, void *level) {
    void *arr = operator new(0xc);
    Array_String_ctor(arr);
    F<void *>(self, 0x10) = arr;
    ArraySetLength_String(5, arr);
    F<int>(self, 0x14) = 0;

    if (**g_APL_apFlag != 0) {
        String *s = (String *)operator new(0xc);
        String *txt = GameText_getText(*g_APL_gametext, 0x222);
        char a[12], b[12], c[12];
        String_ctor_cstr(b, kApLit1, false);
        String_plus(c, txt, b);
        Station_getName(a, *g_APL_status);
        String_plus(s, c, a);
        entryData(self)[0] = s;
        StringDtorFn d = g_APL_strDtor;
        d(a);
        d(c);
        d(b);
        F<int>(self, 0x14) = F<int>(self, 0x14) + 1;
    }

    void *status = *g_APL_status;
    if (SolarSystem_currentOrbitHasWarpGate(Status_getSystem(status)) != 0) {
        String *s = (String *)operator new(0xc);
        String_ctor_copy(s, GameText_getText(*g_APL_gametext, 0x223), false);
        entryData(self)[1] = s;
        F<int>(self, 0x14) = F<int>(self, 0x14) + 1;
    }

    if (Status_inEmptyOrbit(status) == 0) {
        String *s = (String *)operator new(0xc);
        char a[12], b[12], c[12];
        Station_getName(c, Status_getStation(status));
        String_ctor_cstr(b, kApLit2, false);
        String_plus(a, b, c);
        String_plus(s, a, GameText_getText(*g_APL_gametext, 0x88));
        entryData(self)[2] = s;
        StringDtorFn d = g_APL_strDtor;
        d(a);
        d(c);
        d(b);
        F<int>(self, 0x14) = F<int>(self, 0x14) + 1;
    }

    String *cancel = (String *)operator new(0xc);
    String_ctor_copy(cancel, GameText_getText(*g_APL_gametext, 0x225), false);
    entryData(self)[3] = cancel;
    F<int>(self, 0x14) = F<int>(self, 0x14) + 1;

    if (PlayerEgo_getRoute(Level_getPlayer(level)) != 0) {
        void *route = PlayerEgo_getRoute(Level_getPlayer(level));
        if (*((uint8_t *)Route_getLastWaypoint(route) + 0x130) == 0) {
            String *s = (String *)operator new(0xc);
            String_ctor_copy(s, GameText_getText(*g_APL_gametext, 0x23d), false);
            entryData(self)[4] = s;
            F<int>(self, 0x14) = F<int>(self, 0x14) + 1;
        }
    }

    F<int>(self, 0xc) = 0;
    F<int>(self, 0x0) = 0;
    void *font = *g_APL_font;
    void *canvas = *g_APL_canvas;
    int width = 0;
    Array<void *> *entries = F<Array<void *> *>(self, 0x10);
    for (uint32_t i = 0; i < entries->length; i++) {
        if (entries->data[i] != 0) {
            int w = PaintCanvas_GetTextWidth(*(void **)canvas, (String *)*(void **)font) + 0x13;
            width = F<int>(self, 0xc);
            if (width < w) {
                F<int>(self, 0xc) = w;
                width = w;
            }
        }
        entries = F<Array<void *> *>(self, 0x10);
    }

    int screenH = **g_APL_screenH;
    F<int>(self, 0x4) = (**g_APL_screenW - width) / 2;
    F<int>(self, 0x8) = (screenH + F<int>(self, 0x14) * -0xf - 0xc) / 2;
    while (entries->data[F<int>(self, 0x0)] == 0) {
        _ZN13AutoPilotList4downEv(self);
        entries = F<Array<void *> *>(self, 0x10);
    }
}
