#include "gof2/AutoPilotList.h"
#include "gof2/GameText.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Route.h"
#include "gof2/SolarSystem.h"
#include "gof2/Station.h"
#include "gof2/String.h"


extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);

// ---- touch_1314d0.cpp ----
extern "C" void _ZN13AutoPilotList4downEv(AutoPilotList *self);   // AutoPilotList::down()

// AutoPilotList::touch(int p1, int p2) -> selected index, or -1 if the touch is outside
// the list window. On a hit, resets the selection to the top then steps down to the row
// that was touched.
int AutoPilotList::touch(int p1, int p2) {
    AutoPilotList *self = this;
    int row;
    if (p1 < self->field_0x4 ||
        self->field_0x4 + self->field_0xc <= p1 ||
        (row = (p2 - self->field_0x8 - 0xe) / 0xf, p2 - self->field_0x8 < -0xf) ||
        (uint32_t)(row + 1) >= ((Array<void *> *)self->field_0x10)->size()) {
        return -1;
    }
    self->field_0x0 = 0;
    for (int i = 0; i <= row; i++)
        _ZN13AutoPilotList4downEv(self);
    return self->field_0x0;
}

// ---- _AutoPilotList_131378.cpp ----
// AutoPilotList::~AutoPilotList() - release the owned String* array, free it, null it.
// Mangled name so the demangled symbol contains "~AutoPilotList".
extern "C" void ArrayReleaseClasses_String(void *arr);   // blx 0x6facc
extern "C" void *Array_String_dtor(void *arr);            // blx 0x6f64c
extern "C" void operator_delete(void *p);                 // blx 0x6eb48

extern "C" AutoPilotList *_ZN13AutoPilotListD1Ev(AutoPilotList *self) {
    ArrayReleaseClasses_String(self->field_0x10);
    void *a = self->field_0x10;
    if (a != 0)
        operator_delete(Array_String_dtor(a));
    self->field_0x10 = 0;
    return self;
}

// ---- down_13135c.cpp ----
// AutoPilotList::down() - advance selection to the next non-empty entry, wrapping at 4.
extern "C" void _ZN13AutoPilotList4downEv(AutoPilotList *self) {
    void **data = ((Array<void *> *)self->field_0x10)->data();
    int i = self->field_0x0;
    int n;
    do {
        n = 0;
        if (i < 4)
            n = i + 1;
        i = n;
    } while (data[i] == 0);
    self->field_0x0 = i;
}

// ---- getTargetString_13139c.cpp ----
// AbyssEngine::String::String(out, String* src, bool) -> void   (0x6f028)
// AbyssEngine::String::String(out, const char* cstr, bool) -> void   (0x6ee18)

// Fallback literal accessed PC-relative.
extern const char kEmpty[] __attribute__((visibility("hidden")));

// AutoPilotList::getTargetString() -> String by value (sret in r0, this in r1).
// Returns a copy of the selected entry's String if the index is in range, else the
// fallback literal.
RetStr AutoPilotList::getTargetString() {
    AutoPilotList *self = this;
    RetStr r;
    int idx = self->field_0x0;
    Array<void *> *entries = ((Array<void *> *)self->field_0x10);
    if (idx >= 0 && (uint32_t)idx < entries->size())
        ((String *)(&r))->ctor_copy(entries->data()[idx], false);
    else
        String_ctor_cstr(&r, kEmpty, false);
    return r;
}

// ---- up_1313cc.cpp ----
// AutoPilotList::up() - move selection to the previous non-empty entry, wrapping at 0.
extern "C" void _ZN13AutoPilotList2upEv(AutoPilotList *self) {
    void **data = ((Array<void *> *)self->field_0x10)->data();
    int i = self->field_0x0;
    int n;
    do {
        n = 4;
        if (0 < i)
            n = i - 1;
        i = n;
    } while (data[i] == 0);
    self->field_0x0 = i;
}

// ---- AutoPilotList_131064.cpp ----
namespace AbyssEngine { struct String; }
using AbyssEngine::String;

// 0x72f70
extern "C" void Array_String_ctor(void *arr);                            // 0x6f628
extern "C" void ArraySetLength_String(int n, void *arr);                 // 0x6fe08
extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);     // 0x6ee18
extern "C" void String_plus(void *out, const void *a, const void *b);    // 0x6ef98 operator+
// 0x736a8
extern "C" void *Status_getSystem(void *status);                         // 0x71a10
// 0x73c18
extern "C" int Status_inEmptyOrbit(void *status);                        // 0x73d20
extern "C" void *Status_getStation(void *status);                        // 0x71c14
extern "C" void *Level_getPlayer(void *level);                           // 0x72034
// 0x768b8
// 0x71ee4
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
    return *(void ***)((char *)self->field_0x10 + 0x4);
}

// AutoPilotList::AutoPilotList(Level*) - build the list of autopilot destinations (current
// station, warp gate, mission target, "cancel", route waypoint), measure the widest entry
// to size the window, center it, then select the first non-empty row.
extern "C" void _ZN13AutoPilotListC1EP5Level(AutoPilotList *self, void *level) {
    void *arr = operator new(0xc);
    Array_String_ctor(arr);
    self->field_0x10 = arr;
    ArraySetLength_String(5, arr);
    self->field_0x14 = 0;

    if (**g_APL_apFlag != 0) {
        String *s = (String *)operator new(0xc);
        String *txt = ((GameText *)(*g_APL_gametext))->getText(0x222);
        char a[12], b[12], c[12];
        String_ctor_cstr(b, kApLit1, false);
        String_plus(c, txt, b);
        ((Station *)(a))->getName();
        String_plus(s, c, a);
        entryData(self)[0] = s;
        StringDtorFn d = g_APL_strDtor;
        d(a);
        d(c);
        d(b);
        self->field_0x14 = self->field_0x14 + 1;
    }

    void *status = *g_APL_status;
    if (((SolarSystem *)(Status_getSystem(status)))->currentOrbitHasWarpGate() != 0) {
        String *s = (String *)operator new(0xc);
        ((String *)(s))->ctor_copy(((GameText *)(*g_APL_gametext))->getText(0x223), false);
        entryData(self)[1] = s;
        self->field_0x14 = self->field_0x14 + 1;
    }

    if (Status_inEmptyOrbit(status) == 0) {
        String *s = (String *)operator new(0xc);
        char a[12], b[12], c[12];
        ((Station *)(c))->getName();
        String_ctor_cstr(b, kApLit2, false);
        String_plus(a, b, c);
        String_plus(s, a, ((GameText *)(*g_APL_gametext))->getText(0x88));
        entryData(self)[2] = s;
        StringDtorFn d = g_APL_strDtor;
        d(a);
        d(c);
        d(b);
        self->field_0x14 = self->field_0x14 + 1;
    }

    String *cancel = (String *)operator new(0xc);
    ((String *)(cancel))->ctor_copy(((GameText *)(*g_APL_gametext))->getText(0x225), false);
    entryData(self)[3] = cancel;
    self->field_0x14 = self->field_0x14 + 1;

    if (((PlayerEgo *)(Level_getPlayer(level)))->getRoute() != 0) {
        void *route = ((PlayerEgo *)(Level_getPlayer(level)))->getRoute();
        if (*((uint8_t *)((Route *)(route))->getLastWaypoint() + 0x130) == 0) {
            String *s = (String *)operator new(0xc);
            ((String *)(s))->ctor_copy(((GameText *)(*g_APL_gametext))->getText(0x23d), false);
            entryData(self)[4] = s;
            self->field_0x14 = self->field_0x14 + 1;
        }
    }

    self->field_0xc = 0;
    self->field_0x0 = 0;
    void *font = *g_APL_font;
    void *canvas = *g_APL_canvas;
    int width = 0;
    Array<void *> *entries = ((Array<void *> *)self->field_0x10);
    for (uint32_t i = 0; i < entries->size(); i++) {
        if (entries->data()[i] != 0) {
            int w = PaintCanvas_GetTextWidth(*(void **)canvas, (String *)*(void **)font) + 0x13;
            width = self->field_0xc;
            if (width < w) {
                self->field_0xc = w;
                width = w;
            }
        }
        entries = ((Array<void *> *)self->field_0x10);
    }

    int screenH = **g_APL_screenH;
    self->field_0x4 = (**g_APL_screenW - width) / 2;
    self->field_0x8 = (screenH + self->field_0x14 * -0xf - 0xc) / 2;
    while (entries->data()[self->field_0x0] == 0) {
        _ZN13AutoPilotList4downEv(self);
        entries = ((Array<void *> *)self->field_0x10);
    }
}

// ---- draw_1313e8.cpp ----
namespace AbyssEngine { struct String; }
using AbyssEngine::String;

// 0x72f70
// 0x6ee30
extern "C" void Layout_drawWindow(void *layout, const String *title, int x, int y,
                                  int w, int h);                          // 0x768d0
extern "C" void PaintCanvas_DrawString(void *canvas, const String *font, void *text,
                                       int x, int y, int shadow);         // 0x6fe14

// PC-relative GOT holders. **holder yields the live object.
__attribute__((visibility("hidden"))) extern void **g_APL_layout_draw;     // -> Layout
__attribute__((visibility("hidden"))) extern void **g_APL_gametext_draw;   // -> GameText
__attribute__((visibility("hidden"))) extern void **g_APL_font_draw;       // -> String* font
__attribute__((visibility("hidden"))) extern void **g_APL_canvas_draw;     // -> PaintCanvas

// AutoPilotList::draw() - draw the window frame plus one row per non-empty entry.
void AutoPilotList::draw() {
    void *layout = *g_APL_layout_draw;
    String *title = ((GameText *)(*g_APL_gametext_draw))->getText(0x23c);
    char tmp[12];
    ((String *)(tmp))->ctor_copy(title, false);
    Layout_drawWindow(layout, (String *)tmp, this->field_0x4, this->field_0x8,
                      this->field_0xc, this->field_0x14 * 0xf + 0x16);
    ((String *)(tmp))->dtor();

    int drawn = 0;
    void **canvasHolder = g_APL_canvas_draw;
    void **fontHolder = g_APL_font_draw;
    for (uint32_t i = 0; i < ((Array<void *> *)this->field_0x10)->size(); i++) {
        void *text = ((Array<void *> *)this->field_0x10)->data()[i];
        if (text != 0) {
            PaintCanvas_DrawString(*canvasHolder, (String *)*fontHolder, text,
                                   this->field_0x4,
                                   drawn * 0xf + this->field_0x8 + 0x12, 0);
            drawn = drawn + 1;
        }
    }
}
