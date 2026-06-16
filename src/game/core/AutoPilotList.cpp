#include "gof2/game/core/AutoPilotList.h"   // also pulls in SolarSystem.h (String + SolarSystem)
#include "gof2/engine/core/GameText.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/world/Route.h"
#include "gof2/game/core/PaintCanvasClass.h"

// Station is declared minimally here rather than via gof2/Station.h: that header defines
// String unconditionally, which would clash with the identical definition already in scope
// from SolarSystem.h within this single TU. Only Station::getName() is used here.
struct Station {
    String getName();
};

// Canonical render canvas singleton (binary .bss 0x2281b8). Declared locally rather than
// via PaintCanvas.h because this TU already defines class PaintCanvas through
// PaintCanvasClass.h; the declaration matches the canonical definition.
extern PaintCanvas* gCanvas;

// GOT-holder game globals (live object obtained via the double indirection).
__attribute__((visibility("hidden"))) extern int** g_APL_apFlag;        // status-active flag
__attribute__((visibility("hidden"))) extern void** g_APL_gametext;     // GameText singleton
__attribute__((visibility("hidden"))) extern void** g_APL_font;         // measurement font String*
__attribute__((visibility("hidden"))) extern int** g_APL_screenW;       // screen width
__attribute__((visibility("hidden"))) extern int** g_APL_screenH;       // screen height
__attribute__((visibility("hidden"))) extern void** g_APL_layout_draw;  // Layout (draw())
__attribute__((visibility("hidden"))) extern void** g_APL_gametext_draw; // GameText (draw())
__attribute__((visibility("hidden"))) extern void** g_APL_font_draw;    // String* font (draw())

extern const char kEmpty[] __attribute__((visibility("hidden")));
extern const char kApLit1[] __attribute__((visibility("hidden")));
extern const char kApLit2[] __attribute__((visibility("hidden")));

// Build the list of autopilot destinations (current station, warp gate, mission target,
// "cancel", route waypoint), measure the widest entry to size the window, center it on
// screen, then select the first non-empty row.
AutoPilotList::AutoPilotList(Level* level) {
    this->entries = new Array<String*>();
    this->entries->resize(5);
    this->count = 0;

    if (**g_APL_apFlag != 0) {
        String b(kApLit1);
        String c = *((GameText*)*g_APL_gametext)->getText(0x222) + b;
        String a = ((Station*)(&a))->getName();
        (*this->entries)[0] = new String(c + a);
        this->count++;
    }

    if (((SolarSystem*)gStatus->getSystem())->currentOrbitHasWarpGate() != 0) {
        String* s = new String;
        s->ctor_copy(((GameText*)*g_APL_gametext)->getText(0x223), false);
        (*this->entries)[1] = s;
        this->count++;
    }

    if (gStatus->inEmptyOrbit() == 0) {
        String c = ((Station*)(&c))->getName();
        String b(kApLit2);
        String a = b + c;
        (*this->entries)[2] = new String(a + *((GameText*)*g_APL_gametext)->getText(0x88));
        this->count++;
    }

    String* cancel = new String;
    cancel->ctor_copy(((GameText*)*g_APL_gametext)->getText(0x225), false);
    (*this->entries)[3] = cancel;
    this->count++;

    if (((PlayerEgo*)((Level*)level)->getPlayer())->getRoute() != 0) {
        Route* route = (Route*)(intptr_t)((PlayerEgo*)((Level*)level)->getPlayer())->getRoute();
        if (*((uint8_t*)route->getLastWaypoint() + 0x130) == 0) {
            String* s = new String;
            s->ctor_copy(((GameText*)*g_APL_gametext)->getText(0x23d), false);
            (*this->entries)[4] = s;
            this->count++;
        }
    }

    this->width = 0;
    this->selected = 0;
    void* font = *g_APL_font;
    for (uint32_t i = 0; i < this->entries->size(); i++) {
        if ((*this->entries)[i] != nullptr) {
            void* fontStr = *(void**)font;
            int w = gCanvas->GetTextWidth(
                        (unsigned int)(uintptr_t)fontStr, fontStr) + 0x13;
            if (this->width < w)
                this->width = w;
        }
    }

    this->x = (**g_APL_screenW - this->width) / 2;
    this->y = (**g_APL_screenH + this->count * -0xf - 0xc) / 2;
    while ((*this->entries)[this->selected] == nullptr)
        this->down();
}

// Release the owned String* array.
AutoPilotList::~AutoPilotList() {
    if (this->entries != nullptr) {
        for (String* s : *this->entries)
            delete s;
        this->entries->clear();
    }
    delete this->entries;
    this->entries = nullptr;
}

// Advance the selection to the next non-empty entry, wrapping at row 4.
void AutoPilotList::down() {
    String** data = this->entries->data();
    int i = this->selected;
    do {
        i = (i < 4) ? i + 1 : 0;
    } while (data[i] == nullptr);
    this->selected = i;
}

// Move the selection to the previous non-empty entry, wrapping at row 0.
void AutoPilotList::up() {
    String** data = this->entries->data();
    int i = this->selected;
    do {
        i = (i > 0) ? i - 1 : 4;
    } while (data[i] == nullptr);
    this->selected = i;
}

// Map a touch point to a row: -1 if outside the list window, otherwise reset the selection
// to the top and step down to the touched row, returning the new selection index.
int AutoPilotList::touch(int px, int py) {
    int row;
    if (px < this->x ||
        this->x + this->width <= px ||
        (row = (py - this->y - 0xe) / 0xf, py - this->y < -0xf) ||
        (uint32_t)(row + 1) >= this->entries->size()) {
        return -1;
    }
    this->selected = 0;
    for (int i = 0; i <= row; i++)
        this->down();
    return this->selected;
}

// Return a copy of the selected entry's label, or the empty fallback if out of range.
String AutoPilotList::getTargetString() {
    int idx = this->selected;
    if (idx >= 0 && (uint32_t)idx < this->entries->size())
        return *(*this->entries)[idx];
    return String(kEmpty);
}

// Draw the window frame plus one row per non-empty entry.
void AutoPilotList::draw() {
    String title;
    title.ctor_copy(((GameText*)*g_APL_gametext_draw)->getText(0x23c), false);
    ((Layout*)*g_APL_layout_draw)->drawWindow(&title, this->x, this->y, this->width,
                                              this->count * 0xf + 0x16);

    int drawn = 0;
    for (uint32_t i = 0; i < this->entries->size(); i++) {
        String* text = (*this->entries)[i];
        if (text != nullptr) {
            gCanvas->DrawString(
                (unsigned int)(uintptr_t)(String*)*g_APL_font_draw, text,
                this->x, drawn * 0xf + this->y + 0x12, false);
            drawn++;
        }
    }
}
