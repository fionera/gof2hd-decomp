#include "gof2/game/core/AutoPilotList.h"   // also pulls in gof2/SolarSystem.h (RetStr + SolarSystem)
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/core/String.h"
#include "gof2/game/core/PaintCanvasClass.h"

// Station is declared minimally here rather than via gof2/Station.h: that header
// defines RetStr unconditionally, which would clash with the identical definition
// from gof2/SolarSystem.h (already in scope) within this single TU. Only Station's
// getName() accessor is used, and the decomp accesses Station through byte-offset
// casts, so a minimal forward declaration is sufficient and ABI-safe.
struct Station {
    RetStr getName();
};

extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);

void _ZN13AutoPilotList4downEv(AutoPilotList *self);   // AutoPilotList::down()

// AutoPilotList::touch(int p1, int p2) -> selected index, or -1 if the touch is outside
// the list window. On a hit, resets the selection to the top then steps down to the row
// that was touched.
int AutoPilotList::touch(int p1, int p2) {
    AutoPilotList *self = this;
    int row;
    if (p1 < self->x ||
        self->x + self->width <= p1 ||
        (row = (p2 - self->y - 0xe) / 0xf, p2 - self->y < -0xf) ||
        (uint32_t)(row + 1) >= self->entries->size()) {
        return -1;
    }
    self->selected = 0;
    for (int i = 0; i <= row; i++)
        _ZN13AutoPilotList4downEv(self);
    return self->selected;
}

// AutoPilotList::~AutoPilotList() - release the owned String* array, free it, null it.
// Mangled name so the demangled symbol contains "~AutoPilotList".
AutoPilotList *_ZN13AutoPilotListD1Ev(AutoPilotList *self) {
    if (self->entries != nullptr) {
        for (String *s : *self->entries)
            delete s;
        self->entries->clear();
    }
    delete self->entries;
    self->entries = nullptr;
    return self;
}

// AutoPilotList::down() - advance selection to the next non-empty entry, wrapping at 4.
void _ZN13AutoPilotList4downEv(AutoPilotList *self) {
    String **data = self->entries->data();
    int i = self->selected;
    int n;
    do {
        n = 0;
        if (i < 4)
            n = i + 1;
        i = n;
    } while (data[i] == 0);
    self->selected = i;
}

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
    int idx = self->selected;
    Array<String *> *entries = self->entries;
    if (idx >= 0 && (uint32_t)idx < entries->size())
        ((String *)(&r))->ctor_copy((*entries)[idx], false);
    else
        String_ctor_cstr(&r, kEmpty, false);
    return r;
}

// AutoPilotList::up() - move selection to the previous non-empty entry, wrapping at 0.
void _ZN13AutoPilotList2upEv(AutoPilotList *self) {
    String **data = self->entries->data();
    int i = self->selected;
    int n;
    do {
        n = 4;
        if (0 < i)
            n = i - 1;
        i = n;
    } while (data[i] == 0);
    self->selected = i;
}

namespace AbyssEngine { struct String; }
using AbyssEngine::String;

extern "C" void String_ctor_cstr(void *out, const char *cstr, bool);
extern "C" void String_plus(void *out, const void *a, const void *b);    // 0x6ef98 operator+
void _ZN13AutoPilotList4downEv(AutoPilotList *self);          // down()

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

static inline String **entryData(AutoPilotList *self) {
    return self->entries->data();
}

// AutoPilotList::AutoPilotList(Level*) - build the list of autopilot destinations (current
// station, warp gate, mission target, "cancel", route waypoint), measure the widest entry
// to size the window, center it, then select the first non-empty row.
void _ZN13AutoPilotListC1EP5Level(AutoPilotList *self, void *level) {
    self->entries = new Array<String*>();
    self->entries->resize(5);
    self->count = 0;

    if (**g_APL_apFlag != 0) {
        String *s = (String *)operator new(0xc);
        String *txt = (String *)((GameText *)(*g_APL_gametext))->getText(0x222);
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
        self->count = self->count + 1;
    }

    void *status = *g_APL_status;
    if (((SolarSystem *)(((Status *)(status))->getSystem()))->currentOrbitHasWarpGate() != 0) {
        String *s = (String *)operator new(0xc);
        ((String *)(s))->ctor_copy((String *)((GameText *)(*g_APL_gametext))->getText(0x223), false);
        entryData(self)[1] = s;
        self->count = self->count + 1;
    }

    if (((Status *)(status))->inEmptyOrbit() == 0) {
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
        self->count = self->count + 1;
    }

    String *cancel = (String *)operator new(0xc);
    ((String *)(cancel))->ctor_copy((String *)((GameText *)(*g_APL_gametext))->getText(0x225), false);
    entryData(self)[3] = cancel;
    self->count = self->count + 1;

    if (((PlayerEgo *)(((Level *)(level))->getPlayer()))->getRoute() != 0) {
        void *route = (void *)(intptr_t)((PlayerEgo *)(((Level *)(level))->getPlayer()))->getRoute();
        if (*((uint8_t *)((Route *)(route))->getLastWaypoint() + 0x130) == 0) {
            String *s = (String *)operator new(0xc);
            ((String *)(s))->ctor_copy((String *)((GameText *)(*g_APL_gametext))->getText(0x23d), false);
            entryData(self)[4] = s;
            self->count = self->count + 1;
        }
    }

    self->width = 0;
    self->selected = 0;
    void *font = *g_APL_font;
    void *canvas = *g_APL_canvas;
    int width = 0;
    Array<String *> *entries = self->entries;
    for (uint32_t i = 0; i < entries->size(); i++) {
        if ((*entries)[i] != 0) {
            void *fontStr = (void *)*(void **)font;
            int w = ((PaintCanvas*)*(void **)canvas)->GetTextWidth((unsigned int)(uintptr_t)fontStr, fontStr) + 0x13;
            width = self->width;
            if (width < w) {
                self->width = w;
                width = w;
            }
        }
        entries = self->entries;
    }

    int screenH = **g_APL_screenH;
    self->x = (**g_APL_screenW - width) / 2;
    self->y = (screenH + self->count * -0xf - 0xc) / 2;
    while ((*entries)[self->selected] == 0) {
        _ZN13AutoPilotList4downEv(self);
        entries = self->entries;
    }
}

namespace AbyssEngine { struct String; }
using AbyssEngine::String;

// PC-relative GOT holders. **holder yields the live object.
__attribute__((visibility("hidden"))) extern void **g_APL_layout_draw;     // -> Layout
__attribute__((visibility("hidden"))) extern void **g_APL_gametext_draw;   // -> GameText
__attribute__((visibility("hidden"))) extern void **g_APL_font_draw;       // -> String* font
__attribute__((visibility("hidden"))) extern void **g_APL_canvas_draw;     // -> PaintCanvas

// AutoPilotList::draw() - draw the window frame plus one row per non-empty entry.
void AutoPilotList::draw() {
    void *layout = *g_APL_layout_draw;
    String *title = (String *)((GameText *)(*g_APL_gametext_draw))->getText(0x23c);
    char tmp[12];
    ((String *)(tmp))->ctor_copy(title, false);
    ((Layout *)(layout))->drawWindow((String *)tmp, this->x, this->y, this->width, this->count * 0xf + 0x16);
    ((String *)(tmp))->dtor();

    int drawn = 0;
    void **canvasHolder = g_APL_canvas_draw;
    void **fontHolder = g_APL_font_draw;
    for (uint32_t i = 0; i < this->entries->size(); i++) {
        String *text = (*this->entries)[i];
        if (text != 0) {
            ((PaintCanvas*)*canvasHolder)->DrawString((unsigned int)(uintptr_t)(String *)*fontHolder, text,
                                   this->x,
                                   drawn * 0xf + this->y + 0x12, false);
            drawn = drawn + 1;
        }
    }
}
