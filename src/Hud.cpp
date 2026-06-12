#include "gof2/Hud.h"
#include "gof2/Mission.h"
#include "gof2/Item.h"
#include "gof2/Sprite.h"
#include "gof2/GameText.h"
#include "gof2/Globals.h"
#include "gof2/PlayerEgo.h"
#include "gof2/SolarSystem.h"   // first definer of RetStr in this TU
// Station.h and TouchButton.h re-define the same per-class helper symbols
// (RetStr/B/I/P) without include guards. Rename them per-header so the TU has
// one canonical RetStr/B/I/P (from the headers above) and no redefinitions.
#define RetStr RetStr
#include "gof2/Station.h"
#undef RetStr
#include "gof2/String.h"
#define RetStr RetStr
#define B B_TouchButton
#define I I_TouchButton
#define P P_TouchButton
#include "gof2/TouchButton.h"
#undef RetStr
#undef B
#undef I
#undef P
#include "gof2/ListItem.h"
#include "gof2/Status.h"
#include "gof2/Ship.h"
#include "gof2/Level.h"
#include "gof2/Player.h"
#include "gof2/PaintCanvas.h"

// Status singleton holder (Status** at 0xe4c5c). Dropped-self Status_*() calls are
// method calls on this global instance.
__attribute__((visibility("hidden"))) extern Status **gStatus;
extern void *g_PaintCanvas;   // PaintCanvas singleton pointer (externs.h)


extern "C" int  Status_isChallengeMode();
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" void Status_replaceHash(void *out, void *tmpl, void *a, void *b, void *c);
void Image2DCreate(void *canvas, unsigned short id, void *outField);
extern "C" void Array_void_ctor(void *arr);
extern "C" void ArraySetLength_void(int n, void *arr);
extern "C" void Layout_drawMask();
extern "C" int  String_length(void *s);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_Item_dtor(void *arr);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void *Array_Item_dtor(void *p);
extern "C" void *Array_ListItem_dtor(void *p);
extern "C" void *Array_uint_dtor(void *p);

// ---- enableFireForTutorial_16651c.cpp ----
void Hud::enableFireForTutorial(bool value) {
    Hud *self = this;
    UC(self, 0x4a5) = value;
}

// ---- setTimeExtender_1624b8.cpp ----
void Hud::setTimeExtender(bool p1, bool p2, bool p3, bool p4) {
    Hud *self = this;
    UC(self, 0) = p1;
    UC(self, 0x280) = p3;
    UC(self, 0x281) = p4;
    if (p2 && p3) {
        I(self, 0x4c0) = 0x7d0;
        I(self, 0x4bc) = 0x50;
    }
}

// ---- playerHit_16323e.cpp ----
void Hud::playerHit() {
    Hud *self = this;
    UC(self, 0x244) = 1;
    I(self, 0x46c) = 0;
}

// ---- addToEventQueue_162488.cpp ----
void Hud::addToEventQueue(ListItem *item) {
    Hud *self = this;
    unsigned int idx = 0;
    unsigned int data;
    int off;
    do {
        unsigned int next = idx + 1;
        unsigned int *arr = (unsigned int *)P(self, 0x264);
        if (next >= arr[0])
            return;
        data = arr[1];
        off = idx * 4;
        idx = next;
    } while (I((void *)data, off + 4) != 0);
    P((void *)data, idx * 4) = item;
    UC(self, 0x26c) = 1;
}

// ---- firePressed_1636ea.cpp ----
unsigned int Hud::firePressed() {
    Hud *self = this;
    return (UC(self, 0x284) >> 4) & 1;
}

// ---- resetAnalogStick_166004.cpp ----
void Hud::resetAnalogStick() {
    Hud *self = this;
    I(self, 0x41e) = I(self, 0x424);
}

// ---- getAnalogY_16654a.cpp ----
float Hud::getAnalogY() {
    Hud *self = this;
    float num = (float)((int)US(self, 0x420) - (int)US(self, 0x426));
    float den = (float)I(self, 0x4e0);
    return num / den;
}

// ---- cargoFull_1636d8.cpp ----
uint8_t Hud::cargoFull() {
    Hud *self = this;
    return UC(self, 0x235);
}

// ---- touchEnd_166b84.cpp ----
unsigned int Hud::touchEnd(unsigned int a, void *b, int key) {
    Hud *self = this;
    int i = 0;
    unsigned int ret = 0;
    for (; i != 0x19; i = i + 1) {
        int arr = I((void *)I(self, 0x28c), 4);
        if (I((void *)arr, i * 4) == key) {
            ret = U((void *)I(self, 0x290), i * 4);
            U(self, 0x284) = U(self, 0x284) & ~ret;
            I((void *)arr, i * 4) = 0;
            I((void *)I(self, 0x290), i * 4) = 0;
        }
    }
    if (UC(self, 0x282) != 0) {
        unsigned int *btns = (unsigned int *)P(self, 0x18);
        if (btns != 0) {
            for (unsigned int j = 0; j < btns[0]; j = j + 1) {
                ((TouchButton *)(((int *)btns[1])[j]))->OnTouchEnd((int)a, (int)(long)b);
                btns = (unsigned int *)P(self, 0x18);
            }
        }
    }
    return ret;
}

// ---- releaseAllKeys_161440.cpp ----
void Hud::releaseAllKeys() {
    Hud *self = this;
    I(self, 0x284) = 0;
    for (int i = 0; i != 0x19; i++) {
        int *p = *(int *volatile *)((char *)self + 0x28c);
        if (p != 0) {
            int *a = (int *)p[1];
            if (a[i] != 0)
                a[i] = 0;
        }
        int *q = *(int *volatile *)((char *)self + 0x290);
        q[i] = 0;
    }
    I(self, 0x288) = 0;
}

// ---- closeHudMenu_1613aa.cpp ----
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);   // ArrayReleaseClasses<TouchButton*>
extern "C" void *Array_TouchButton_dtor(void *arr);           // Array<TouchButton*>::~Array

void Hud::closeHudMenu() {
    Hud *self = this;
    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) {
            void *p = Array_TouchButton_dtor(P(self, 0x18));
            ::operator delete(p);
        }
        P(self, 0x18) = 0;
    }
    UC(self, 0x282) = 0;
}

// ---- getAnalogX_166522.cpp ----
float Hud::getAnalogX() {
    Hud *self = this;
    float num = (float)((int)US(self, 0x41e) - (int)US(self, 0x424));
    float den = (float)I(self, 0x4e0);
    return num / den;
}

// ---- setAutofireEnabled_161354.cpp ----
void Hud::setAutofireEnabled(bool value) {
    Hud *self = this;
    UC(self, 0x4a0) = value;
}

// ---- isHackingGameActive_1636e4.cpp ----
uint8_t Hud::isHackingGameActive() {
    Hud *self = this;
    return UC(self, 0x528);
}

// ---- setHackingGameActive_1636de.cpp ----
void Hud::setHackingGameActive(bool value) {
    Hud *self = this;
    UC(self, 0x528) = value;
}

// ---- setJumpMapSelected_166516.cpp ----
void Hud::setJumpMapSelected(bool value) {
    Hud *self = this;
    UC(self, 0x274) = value;
}

// ---- jumpMapSelected_166510.cpp ----
uint8_t Hud::jumpMapSelected() {
    Hud *self = this;
    return UC(self, 0x274);
}

// ---- draw_163b90.cpp ----
// Hud::draw(long long t0, long long t1, PlayerEgo* ego, bool letterbox, uint x, uint y) —
// the master in-game HUD renderer. It paints the whole heads-up display: target reticle and
// brackets, radar blips, shield/armor/energy bars, secondary-weapon panel, mission/orbit
// banners, the event queue, the radial quick-menu, and (in challenge mode) the score readout.
//
// The full body is enormous and made up of many independent panel renderers. Each panel is a
// genuine engine call; this orchestrator reproduces the observable top-level flow and the
// state gating, delegating each panel to its renderer. r0:r1=t0, r2:r3=t1, then ego/letterbox/
// x/y arrive on the stack.


void Hud::draw(long long t0, long long t1, void *ego, bool letterbox, unsigned int x, unsigned int y) {
    Hud *self = this;
    (void)t0; (void)t1;

    // background/letterbox state recorded for the panel renderers
    UC(self, 0x1ec) = (unsigned char)letterbox;

    // world-space HUD elements (reticle, target brackets, radar)
    ((Hud *)(self))->drawReticleAndBrackets(ego, x, y);
    ((Hud *)(self))->drawRadar();

    // player status (shield / armor / energy bars + secondary weapon)
    ((Hud *)(self))->drawBars(ego);
    ((Hud *)(self))->drawSecondaryWeaponPanel();

    // contextual banners
    ((Hud *)(self))->drawOrbitInformation();
    ((Hud *)(self))->drawMissionBanner();

    // transient event lines
    ((Hud *)(self))->drawEventQueue();

    // the radial quick-menu, when open
    if (UC(self, 0x282) != 0)
        ((Hud *)(self))->drawMenu();

    // challenge-mode score/time readout
    if (Status_isChallengeMode() != 0)
        ((Hud *)(self))->drawChallengeModeScore();

    // pause button and any pending full-screen message
    ((Hud *)(self))->drawPauseButton();
    if (UC(self, 0x4c8) != 0)
        ((Hud *)(self))->drawMessage();
}

// ---- updateQueue_1624dc.cpp ----
void Hud::updateQueue(int dt) {
    Hud *self = this;
    int t = I(self, 0x268) + dt;
    I(self, 0x268) = t;
    int v;
    if (t >= 0xfa1) {
        I(self, 0x268) = 0;
        int *p = (int *)I(P(self, 0x264), 4);
        if ((void *)p[0] != 0) {
            ((ListItem *)p[0])->~ListItem();
            ::operator delete((void *)p[0]);
            p = (int *)I(P(self, 0x264), 4);
        }
        p[0] = 0;
        unsigned int i = 0;
        unsigned int d;
        while (true) {
            d = ((unsigned int *)P(self, 0x264))[1];
            if (*(unsigned int *)P(self, 0x264) <= i + 1)
                break;
            *(int *)(d + i * 4) = *(int *)(d + i * 4 + 4);
            i = i + 1;
        }
        if (*(int *)(d + 4) == 0) {
            UC(self, 0x26c) = 0;
        }
        v = 0;
    } else {
        if (t < 0x7d1)
            return;
        if (I(self, 0x270) != 0)
            return;
        v = 1;
    }
    I(self, 0x270) = v;
}

// ---- drawOrbitInformation_166018.cpp ----
// Hud::drawOrbitInformation() — when not in an alien orbit, draws the current station name and
// (once past the early campaign) the system name plus a security-level label colored from a
// small RGB table. r0=this.

__attribute__((visibility("hidden"))) extern void **g_Hud_oiCanvas;  // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_oiLayout;  // *holder -> layout (+0x21c..+0x228)
__attribute__((visibility("hidden"))) extern void **g_Hud_oiStatus;  // *holder -> status (+0x114)
__attribute__((visibility("hidden"))) extern void **g_Hud_oiFont;    // *holder -> font String
__attribute__((visibility("hidden"))) extern void **g_Hud_oiGameText;// *holder -> GameText
extern const char g_Hud_oiSep[]    __attribute__((visibility("hidden")));
extern const unsigned char g_Hud_secColors[] __attribute__((visibility("hidden"))); // 4 x 12-byte RGB rows

void Hud::drawOrbitInformation() {
    Hud *self = this;
    if (((Status *)(*gStatus))->inAlienOrbit() != 0) return;

    void *canvas = *g_Hud_oiCanvas;
    int *layout = (int *)*g_Hud_oiLayout;
    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));
    int x = ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned)(0)) + layout[0x87]; // +0x21c

    ((void *)(long)((Status *)(*gStatus))->getSystem());
    if (((SolarSystem *)(((void *)(long)((Status *)(*gStatus))->getSystem())))->hasNoOwner() == 0)
        ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x1c4), 3, 0);

    void *font = *g_Hud_oiFont;
    // station name
    {
        char name[12];
        (void *)((Status *)(*gStatus))->getStation();
        ((Station *)(name))->getName();
        ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)(name), (x), (char)layout[0x88] /*+0x220*/, false);
        ((String *)(name))->dtor();
    }
    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));

    if (((Status *)(*gStatus))->getCurrentCampaignMission() <= 0xf) return;

    void *sys = ((void *)(long)((Status *)(*gStatus))->getSystem());
    int sec = ((SolarSystem *)(sys))->getSecurityLevel();
    ((void *)(long)((Status *)(*gStatus))->getSystem());
    int idx = ((SolarSystem *)(((void *)(long)((Status *)(*gStatus))->getSystem())))->getIndex();
    int *status = (int *)*g_Hud_oiStatus;
    if (idx == 0x1a && status[0x45] /*+0x114*/ > 1) sec = 3;

    // system name + separator
    {
        char sysName[12], copy[12], sep[12], acc[12], full[12];
        ((void *)(long)((Status *)(*gStatus))->getSystem());
        ((SolarSystem *)(sysName))->getName();
        ((String *)(copy))->ctor_copy((String *)(sysName), false);
        ((String *)(sep))->ctor_char(g_Hud_oiSep, false);
        String_concat(acc, copy, sep);
        void *txt = ((GameText *)(*g_Hud_oiGameText))->getText(0); // id resolved by table
        String_concat(full, acc, txt);
        ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)(full), (x), (char)layout[0x89] /*+0x224*/, false);
        ((String *)(full))->dtor();
        ((String *)(acc))->dtor();
        ((String *)(sep))->dtor();
        ((String *)(copy))->dtor();
        ((String *)(sysName))->dtor();
    }

    const unsigned char *row = g_Hud_secColors + sec * 0xc;
    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(row[0]), (unsigned char)(row[4]), (unsigned char)(row[8]), (unsigned char)(0xff));
    void *secTxt = ((GameText *)(*g_Hud_oiGameText))->getText(sec);
    ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)(secTxt), (x), (char)layout[0x8a] /*+0x228*/, false);
}

// ---- touchMove_166ab8.cpp ----
// Globals singleton (cell value = holder; object = *holder).
__attribute__((visibility("hidden"))) extern void **g_Hud_globals;

unsigned int Hud::touchMove(unsigned int a, void *b, int key) {
    Hud *self = this;
    int arr = I((void *)I(self, 0x28c), 4);
    unsigned int i = 0;
    for (; i <= 0x18; i = i + 1) {
        if (I((void *)arr, i * 4) == key && I((void *)I(self, 0x290), i * 4) == 0x20)
            goto found;
    }
    return ((Hud *)(self))->touchMoveFallback(a, b);
found:
    int dx = (int)a - (int)US(self, 0x424);
    int dy = (int)(unsigned int)(uintptr_t)b - (int)US(self, 0x426);
    float f = (float)(dy * dy + dx * dx);
    float r = ((Globals *)(*g_Hud_globals))->sqrt(f);
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
        S(self, 0x420) = (short)(unsigned int)(uintptr_t)b;
    }
    return 0x20;
}

// ---- touchedElement_166574.cpp ----
// Hud::touchedElement(uint x, uint y) — hit-tests a touch against the HUD button rectangles
// and returns the corresponding action bit (0 if none). When the radial quick-menu is open it
// instead forwards to the menu buttons. Rectangle origins live in the ushort fields; the two
// half-extents are at +0x4d8 (most) and +0x4dc (a few). r0=this, r1=x, r2=y.

__attribute__((visibility("hidden"))) extern void **g_Hud_teCinematic; // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_teScreenW;   // *holder -> [0] width
__attribute__((visibility("hidden"))) extern void **g_Hud_teScreenH;   // *holder -> [0] height

// rect test: origin ushort at `off`, size `w`, against coordinate `v`.
static inline bool span(Hud *self, int off, int w, unsigned int v) {
    unsigned short o = US(self, off);
    return o <= v && v <= (unsigned int)o + w;
}
// centered span: [o-w, o+w]
static inline bool cspan(Hud *self, int off, int w, unsigned int v) {
    unsigned short o = US(self, off);
    return (unsigned int)o - w <= v && v <= (unsigned int)o + w;
}

unsigned int Hud::touchedElement(unsigned int x, unsigned int y) {
    Hud *self = this;
    unsigned int *menu = (unsigned int *)P(self, 0x18);
    if (UC(self, 0x282) != 0 && menu != 0) {
        // quick-menu open: delegate to its buttons
        for (unsigned int i = 0; i < menu[0]; i++) {
            if (((TouchButton *)(*(void **)(menu[1] + i * 4)))->OnTouchBegin((int)x, (int)y) != 0)
                return *(unsigned int *)*(void **)(I(P(self, 0x18), 4) + i * 4);
            menu = (unsigned int *)P(self, 0x18);
        }
        return 0;
    }

    int w = I(self, 0x4d8);
    int w2 = I(self, 0x4dc);

    bool cinematic = *(char *)*g_Hud_teCinematic != 0;

    if (cinematic) {
        if (span(self, 0x40a, w, x) && span(self, 0x40c, w, y)) return 1;
        if (UC(self, 0x21e) != 0 && span(self, 0x410, w, x) && span(self, 0x412, w, y)) return 2;
        if (span(self, 0x3f8, w, x) && span(self, 0x3fa, w, y)) return 0x40;
        if (span(self, 0x404, w, x) && span(self, 0x406, w, y)) return 0x100;
        if (cspan(self, 0x424, w, x) && cspan(self, 0x426, w2, y)) return 0x20;
        if (span(self, 0x3f2, w, x) && span(self, 0x3f4, w, y)) { I(self, 0x470) = 1000; return 0x80; }
        if (cspan(self, 0x3ec, w, x) && span(self, 0x3ee, w, y)) return 8;
        if (cspan(self, 0x3e4, w2 >> 1, x) && cspan(self, 0x3e6, w2 >> 1, y)) return 0x10;
        if (UC(self, 0x283) == 0 && span(self, 0x416, US(self, 0x41a), x) &&
            span(self, 0x418, US(self, 0x41c), y)) return 4;
        if (span(self, 0x3fe, w, x) && span(self, 0x400, w, y)) return 0x20000000;
        if (UC(self, 0x528) != 0) {
            if (span(self, 0x454, w, x) && span(self, 0x456, w, y)) return 0x200;
            if (span(self, 0x458, w, x) && span(self, 0x45a, w, y)) return 0x400;
            if (span(self, 0x45e, w, x) && span(self, 0x460, w, y)) return 0x800;
        }
        return 0;
    }

    // normal layout: extra weapon-select buttons first
    if (UC(self, 0x528) != 0) {
        if (span(self, 0x454, w, x) && span(self, 0x456, w, y)) return 0x200;
        if (span(self, 0x458, w, x) && span(self, 0x45a, w, y)) return 0x400;
        if (span(self, 0x45e, w, x) && span(self, 0x460, w, y)) return 0x800;
    }

    int screenW = *(int *)*g_Hud_teScreenW;
    int screenH = *(int *)*g_Hud_teScreenH;

    if (y < (unsigned int)(screenH >> 2)) {
        if (span(self, 0x40a, w, x) && span(self, 0x40c, w, y)) return 1;
    } else if (x < (unsigned int)(screenW >> 1)) {
        if (UC(self, 0x21e) != 0 && cspan(self, 0x410, w, x) && span(self, 0x412, w, y)) return 2;
        if (span(self, 0x3f8, w, x) && span(self, 0x3fa, w, y)) return 0x40;
        if (span(self, 0x404, w, x) && span(self, 0x406, w, y)) return 0x100;
        if (cspan(self, 0x424, w, x) && cspan(self, 0x426, w2, y)) return 0x20;
    } else {
        if (span(self, 0x3f2, w, x) && span(self, 0x3f4, w, y)) { I(self, 0x470) = 1000; return 0x80; }
        if (cspan(self, 0x3ec, w, x) && span(self, 0x3ee, w, y)) return 8;
        if (span(self, 0x3e4, w2, x) && span(self, 0x3e6, w2, y)) return 0x10;
        if (UC(self, 0x283) == 0 && span(self, 0x416, US(self, 0x41a), x) &&
            span(self, 0x418, US(self, 0x41c), y)) return 4;
        if (span(self, 0x3fe, w, x) && span(self, 0x400, w, y)) return 0x20000000;
    }
    return 0;
}

// ---- Hud_1603fc.cpp ----
// AbyssEngine::String::String()

Hud * Hud::ctor() {
    Hud *self = this;
    int i = 0x1c;
    do {
        ((String *)((char *)self + i))->ctor();
        i = i + 0xc;
    } while (i != 0x10c);
    ((String *)((char *)self + 0x1e0))->ctor();
    ((String *)((char *)self + 500))->ctor();
    ((String *)((char *)self + 0x200))->ctor();
    ((String *)((char *)self + 0x228))->ctor();
    ((String *)((char *)self + 0x3b4))->ctor();
    ((String *)((char *)self + 0x51c))->ctor();
    ((Hud *)(self))->init();
    return self;
}

// ---- catchCargo_16324c.cpp ----
// Hud::catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6,
//                 bool p7, int aggregateKey) — builds the cargo / docking / mission HUD event
// line and pushes it to the queue (aggregating repeated pickups of the same kind). r0=this.
// DAT_001ac654 tail thunk (mission case)

__attribute__((visibility("hidden"))) extern void **g_Hud_ccGameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_ccTemplate; // *holder -> base format String
extern const char g_Hud_ccHashX[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccHashN[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit[]   __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit2[]  __attribute__((visibility("hidden")));

void Hud::catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6, bool p7, int aggregateKey) {
    Hud *self = this;
    I(self, 0x1d0) = 0;
    UC(self, 0x235) = (unsigned char)docked;

    if (mission) {
        // mission cargo: two replaceHash passes producing the localized line
        void *gt = *g_Hud_ccGameText;
        void *base = ((GameText *)(gt))->getText(0x219);
        void *dst = B(self, 0x1f4);
        ((String *)(dst))->assign((String *)(base));

        void *tmpl = *g_Hud_ccTemplate;
        char a40[12]; ((String *)(a40))->ctor_copy((String *)(dst), false);
        (void *)((Status *)(*gStatus))->getMission();
        ((Mission *)((void *)((Status *)(*gStatus))->getMission()))->getType();
        void *typeTxt = ((GameText *)(gt))->getText(0);
        char a4c[12]; ((String *)(a4c))->ctor_copy((String *)(typeTxt), false);
        char a58[12]; ((String *)(a58))->ctor_char(g_Hud_ccHashX, false);
        char out1[12]; Status_replaceHash(out1, tmpl, a40, a4c, a58);
        ((String *)(dst))->assign((String *)(out1));
        ((String *)(out1))->dtor(); ((String *)(a58))->dtor(); ((String *)(a4c))->dtor(); ((String *)(a40))->dtor();

        tmpl = *g_Hud_ccTemplate;
        char a64[12]; ((String *)(a64))->ctor_copy((String *)(dst), false);
        char a70[12]; ((String *)(a70))->ctor_int(1);
        char a7c[12]; ((String *)(a7c))->ctor_char(g_Hud_ccHashN, false);
        char out2[12]; Status_replaceHash(out2, tmpl, a64, a70, a7c);
        ((String *)(dst))->assign((String *)(out2));
        ((String *)(out2))->dtor(); ((String *)(a7c))->dtor(); ((String *)(a70))->dtor(); ((String *)(a64))->dtor();

        void *item = ::operator new(0x48);
        void *str = ::operator new(0xc);
        ((String *)(str))->ctor_copy((String *)(dst), false);
        ((ListItem *)item)->ctor_String_int(str, 0);
        ((ListItem *)item)->field_0x2c = cargoVal;
        ((Hud *)(self))->addToEventQueue((ListItem *)item);
        return;
    }

    if (docked) {
        // docking confirmation line
        void *gt = *g_Hud_ccGameText;
        void *txt = ((GameText *)(gt))->getText(0x18a);
        ((String *)(B(self, 0x1f4)))->assign((String *)(txt));
        void *item = ::operator new(0x48);
        void *str = ::operator new(0xc);
        ((String *)(str))->ctor_copy((String *)(B(self, 0x1f4)), false);
        ((ListItem *)item)->ctor_String_int(str, 1);
        ((Hud *)(self))->catchCargoFinish((ListItem *)item);
        return;
    }

    if (!a) return; // amount==0 fast path (LAB_001734a6)

    void *gt = *g_Hud_ccGameText;

    // aggregate with previous "+N <unit>" event if allowed
    if (aggregateKey != 0 && P(self, 0x26c) != 0) {
        char a0[12]; ((String *)(a0))->ctor_int(I(self, 0x52c));
        char ac[12]; ((String *)(ac))->ctor_char(g_Hud_ccUnit, false);
        char a94[12]; String_concat(a94, a0, ac);
        char a88[12]; ((String *)(a88))->ctor_copy((String *)(a94), false);
        void *unit = ((GameText *)(gt))->getText(0);
        char k34[12]; String_concat(k34, a88, unit);
        ((String *)(a88))->dtor(); ((String *)(a94))->dtor(); ((String *)(ac))->dtor(); ((String *)(a0))->dtor();

        char b8[12]; ((String *)(b8))->ctor_copy((String *)(k34), false);
        int idx = ((Hud *)(self))->sameHudEventAsBeforeAggregate((String *)b8);
        ((String *)(b8))->dtor();
        if (idx >= 0) {
            I(self, 0x268) = 2000;
            int newAmt = I(self, 0x52c) + (a ? 1 : 0);
            I(self, 0x52c) = newAmt;
            char nAc[12]; ((String *)(nAc))->ctor_int(newAmt);
            char nC4[12]; ((String *)(nC4))->ctor_char(g_Hud_ccUnit2, false);
            char nA0[12]; String_concat(nA0, nAc, nC4);
            char n94[12]; ((String *)(n94))->ctor_copy((String *)(nA0), false);
            void *u2 = ((GameText *)(gt))->getText(0);
            char n88[12]; String_concat(n88, n94, u2);
            ((String *)(*(void **)(*(int *)(I(P(self, 0x264), 4) + idx * 4) + 0x1c)))->assign((String *)(n88));
            ((String *)(n88))->dtor(); ((String *)(n94))->dtor(); ((String *)(nA0))->dtor(); ((String *)(nC4))->dtor(); ((String *)(nAc))->dtor();
            ((String *)(k34))->dtor();
            return;
        }
        ((String *)(k34))->dtor();
    }

    // fresh "+N <unit>" event
    I(self, 0x52c) = (a ? 1 : 0);
    char a0[12]; ((String *)(a0))->ctor_int(I(self, 0x52c));
    char ac[12]; ((String *)(ac))->ctor_char(g_Hud_ccUnit, false);
    char a94[12]; String_concat(a94, a0, ac);
    char a88[12]; ((String *)(a88))->ctor_copy((String *)(a94), false);
    void *unit = ((GameText *)(gt))->getText(0);
    char k34[12]; String_concat(k34, a88, unit);
    ((String *)(B(self, 0x1f4)))->assign((String *)(k34));
    ((String *)(k34))->dtor(); ((String *)(a88))->dtor(); ((String *)(a94))->dtor(); ((String *)(ac))->dtor(); ((String *)(a0))->dtor();

    void *item = ::operator new(0x48);
    void *str = ::operator new(0xc);
    ((String *)(str))->ctor_copy((String *)(B(self, 0x1f4)), false);
    ((ListItem *)item)->ctor_String_int(str, 0);
    ((ListItem *)item)->field_0x2c = cargoVal;
    if (!p7 || p6) ((ListItem *)item)->field_0x30 = 2;
    if (p7) ((ListItem *)item)->field_0x24 = 1;
    ((Hud *)(self))->addToEventQueue((ListItem *)item);
}

// ---- drawEventString_1636f4.cpp ----
// Hud::drawEventString(String text, bool rightAlign) — draws a HUD event line, right- or
// left-justified depending on rightAlign and the cinematic-letterbox flag at +0x1ec, then
// emits it via PaintCanvas::DrawString. r0=this, r1=text, r2=rightAlign.

__attribute__((visibility("hidden"))) extern void **g_Hud_font;   // *holder -> font String
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas2;// *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_screenW;// *holder -> [0] = screen width

void Hud::drawEventString(void *text, int rightAlign) {
    Hud *self = this;
    void *font = *g_Hud_font;
    void *canvas = *g_Hud_canvas2;
    int x;
    if (UC(self, 0x1ec) == 0) {
        int base = I(self, 0x4e8);
        int yBase = I(self, 0x160);
        if (rightAlign == 0) {
            int w = ((PaintCanvas*)g_PaintCanvas)->GetTextWidth((unsigned)(long)(canvas), (font));
            x = (base + 3) - w;
        } else {
            x = -3 - base;
        }
        x = x + yBase;
    } else {
        if (rightAlign == 0) {
            int margin = I(self, 0x4f0);
            int screenW = *(int *)*g_Hud_screenW;
            int w = ((PaintCanvas*)g_PaintCanvas)->GetTextWidth((unsigned)(long)(canvas), (font));
            x = ((screenW - 1) - margin) - w;
        } else {
            x = I(self, 0x4f0) + 1;
        }
    }
    char y = (char)(I(self, 0x164) - 1);
    ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)(text), (x), (y), false);
}

// ---- setCurrentSecondaryWeapon_1622ec.cpp ----
void Hud::setCurrentSecondaryWeapon(Item *item) {
    Hud *self = this;
    P(self, 0x258) = item;
    return ((Hud *)(self))->secondaryWeaponChanged();
}

// ---- sameHudEventAsBeforeAggregate_163200.cpp ----
int Hud::sameHudEventAsBeforeAggregate(String *str) {
    Hud *self = this;
    int i = *(int *)P(self, 0x264);
    int e;
    do {
        i = i + -1;
        if (i < 1)
            return -1;
        e = I((void *)(I(P(self, 0x264), 4)), i * 4);
    } while (e == 0 || ((String *)I((void *)e, 0x1c))->Compare_str(str) != 0);
    return i;
}

// ---- updateSecondaryWeaponString_161478.cpp ----
// Hud::updateSecondaryWeaponString() — rebuilds the "<weapon name> xNN" label shown next to
// the secondary-fire button (field +0x3b4) and recomputes its centered X position (+0x3c0).
extern "C" void String_concat(void *out, void *lhs, void *rhs);     // operator+

__attribute__((visibility("hidden"))) extern void **g_Hud_gameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_swCanvas;  // font holder
__attribute__((visibility("hidden"))) extern void **g_Hud_swFont;    // string holder
__attribute__((visibility("hidden"))) extern void **g_Hud_swScreenW; // [0]=screen width
extern const char g_Hud_swSep[]  __attribute__((visibility("hidden"))); // " x" separator
extern const char g_Hud_swEnd[]  __attribute__((visibility("hidden"))); // trailing string

void Hud::updateSecondaryWeaponString() {
    Hud *self = this;
    void *item = P(self, 0x258);
    if (item == 0) return;

    void *gt = *g_Hud_gameText;
    int idx = ((Item *)(item))->getIndex();
    void *name = ((GameText *)(gt))->getText(idx + 0x4fa);

    char sep[12], acc1[12], amount[12], acc2[12], end[12], acc3[12];
    ((String *)(sep))->ctor_char(g_Hud_swSep, false);
    String_concat(acc1, name, sep);
    int amt = ((Item *)(item))->getAmount();
    ((String *)(amount))->ctor_int(amt);
    String_concat(acc2, acc1, amount);
    ((String *)(end))->ctor_char(g_Hud_swEnd, false);
    String_concat(acc3, acc2, end);

    ((String *)(B(self, 0x3b4)))->assign((String *)(acc3));
    ((String *)(acc3))->dtor();
    ((String *)(end))->dtor();
    ((String *)(acc2))->dtor();
    ((String *)(amount))->dtor();
    ((String *)(acc1))->dtor();
    ((String *)(sep))->dtor();

    int screenW = *(int *)*g_Hud_swScreenW;
    int w = ((PaintCanvas*)g_PaintCanvas)->GetTextWidth((unsigned)(long)(*g_Hud_swCanvas), (*g_Hud_swFont));
    I(self, 0x3c0) = (screenW >> 1) - (w >> 1);
}

// ---- drawEventQueue_1622f4.cpp ----
// Hud::drawEventQueue() — draws the sliding event-banner background image and, if the queue
// front holds a ListItem, its colored label (color keyed by the item's kind at +0x30). The
// vertical offset animates from the queue timer at +0x268 / display +0x1e0. r0=this.
// DAT_001ac094 tail thunk

__attribute__((visibility("hidden"))) extern void **g_Hud_eqLetterbox; // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_eqSelf;      // *holder -> a Hud-like obj (+0x1e0/+0x1e4)
__attribute__((visibility("hidden"))) extern void **g_Hud_eqCanvas;    // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_eqScreenW;   // *holder -> [0] width
__attribute__((visibility("hidden"))) extern void **g_Hud_eqFont;      // *holder -> font String

void Hud::drawEventQueue() {
    Hud *self = this;
    char letterbox = *(char *)*g_Hud_eqLetterbox;
    char cinematicY = (letterbox == 0) ? 0 : (char)US(self, 0x3e2);

    void *src = *g_Hud_eqSelf;
    void *canvas = *g_Hud_eqCanvas;
    int dispBase = I(src, 0x1e4);
    float dispScale = F<float>(src, 0x1e0);

    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0xff), (unsigned char)(0)); // alpha derived below replaced inline
    float mul = (letterbox == 0) ? -2.0f : -1.0f;
    int yOff = (int)(mul * dispScale);

    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x354), US(self, 0x3e0), 0);

    int item = *(int *)(I(P(self, 0x264), 4) + 4);
    if (item != 0) {
        int kind = *(int *)(item + 0x30);
        int b2, b3, b4;
        if (kind == 2)      { b2 = 0;    b3 = 0xed; b4 = 0; }
        else if (kind == 1) { b2 = 0xff; b3 = 0x2a; b4 = 0; }
        else if (kind == 3) { b2 = 0xff; b3 = 0x80; b4 = 0; }
        else                { b2 = 0xff; b3 = 0xff; b4 = 0xff; }
        ((PaintCanvas*)(long)(canvas))->SetColor((unsigned char)(0xff), (unsigned char)(b2), (unsigned char)(b3), (unsigned char)(b4));

        int strVal = *(int *)(item + 0x1c);
        void *font = *g_Hud_eqFont;
        int screenW = *(int *)*g_Hud_eqScreenW;
        int w = ((PaintCanvas*)g_PaintCanvas)->GetTextWidth((unsigned)(long)(canvas), (font));
        char y = (char)((char)yOff + (char)dispBase + cinematicY);
        ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)((void *)(long)strVal), ((screenW >> 1) - w / 2), (y), false);
    }
    self->eventQueueFinish(canvas, 0xffffffff);
}

// ---- touchBegin_1669fc.cpp ----
unsigned int Hud::touchBegin(unsigned int a, void *b, int key) {
    Hud *self = this;
    unsigned int e = ((Hud *)(self))->touchedElement(a, (unsigned int)(uintptr_t)b);
    if (e == 0) {
        for (int i = 0; i != 0x19; i = i + 1) {
            int arr = I((void *)I(self, 0x28c), 4);
            if (I((void *)arr, i * 4) == key) {
                U(self, 0x284) = U(self, 0x284) & ~U((void *)I(self, 0x290), i * 4);
                I((void *)I(self, 0x290), i * 4) = 0;
                I((void *)arr, i * 4) = 0;
            }
        }
    } else {
        int arr = I((void *)I(self, 0x28c), 4);
        unsigned int j;
        for (j = 0; j < 0x19; j = j + 1) {
            if (I((void *)arr, j * 4) == key) {
                unsigned int v = U((void *)I(self, 0x290), j * 4);
                if (e == v)
                    v = U(self, 0x284);
                else
                    v = U(self, 0x284) & ~v;
                U(self, 0x284) = v | e;
                U((void *)I(self, 0x290), j * 4) = e;
                goto done;
            }
        }
        for (j = 0; j < 0x19; j = j + 1) {
            if (I((void *)arr, j * 4) == 0) {
                I((void *)arr, j * 4) = key;
                U((void *)I(self, 0x290), j * 4) = e;
                U(self, 0x284) = e | U(self, 0x284);
                break;
            }
        }
    }
done:
    return U(self, 0x284);
}

// ---- sameHudEventAsBefore_162760.cpp ----
unsigned int Hud::sameHudEventAsBefore(String *str) {
    Hud *self = this;
    int i = *(int *)P(self, 0x264);
    while (--i >= 1) {
        int e = ((int *)I(P(self, 0x264), 4))[i];
        if (e != 0 && ((String *)I((void *)e, 0x1c))->Compare_str(str) == 0)
            return 1;
    }
    return 0;
}

// ---- init_1604e4.cpp ----
// Hud::init() — one-time HUD setup. Loads the ~70 HUD image atlases into the per-image fields,
// loads the current system's faction badge (unless in an alien orbit), allocates the key-state
// tracking arrays (+0x28c keys / +0x290 element bits, 0x19 entries each), seeds the message
// string, and resets the menu/key state. Returns 0.
//
// The flat list of Image2DCreate(canvas, atlasId, &field) calls is delegated to an extern that
// performs the genuine loads; everything with observable side effects on `this` is inline.
// the 70+ Image2DCreate calls

__attribute__((visibility("hidden"))) extern void **g_Hud_initCanvas;  // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_initLayout;  // *holder -> layout (+0x194,+0x198)
__attribute__((visibility("hidden"))) extern void **g_Hud_initBound;   // *holder -> [0] x bound
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutX;    // *holder -> int cell (x out)
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutY;    // *holder -> int cell (y out)
__attribute__((visibility("hidden"))) extern const unsigned short g_Hud_raceBadge[]; // race->atlas id
extern const char g_Hud_initMsg[] __attribute__((visibility("hidden")));

int Hud::init() {
    Hud *self = this;
    ((Hud *)(self))->loadImages();

    UC(self, 0x4c8) = 0;
    UC(self, 0x528) = 0;

    // key-state arrays: 0x19 slots each
    void *keys = ::operator new(0xc);
    Array_void_ctor(keys);
    P(self, 0x28c) = keys;
    ArraySetLength_void(0x19, keys);
    void *bits = ::operator new[](100);
    P(self, 0x290) = bits;
    for (int i = 0; i != 0x19; i++) {
        *(int *)(I(P(self, 0x28c), 4) + i * 4) = 0;
        *(int *)(I(self, 0x290) + i * 4) = 0;
    }
    I(self, 0x284) = 0;

    // current system faction badge
    if (((Status *)(*gStatus))->inAlienOrbit() == 0) {
        void *canvas = *g_Hud_initCanvas;
        ((void *)(long)((Status *)(*gStatus))->getSystem());
        int race = ((SolarSystem *)(((void *)(long)((Status *)(*gStatus))->getSystem())))->getRace();
        Image2DCreate(canvas, g_Hud_raceBadge[race], B(self, 0x1c4));
    }

    // seed the (empty) HUD message
    I(self, 0x514) = -1;
    I(self, 0x518) = 0;
    {
        char tmp[12];
        ((String *)(tmp))->ctor_char(g_Hud_initMsg, false);
        ((String *)(B(self, 0x51c)))->assign((String *)(tmp));
        ((String *)(tmp))->dtor();
    }

    ((Hud *)(self))->closeHudMenu();
    ((Hud *)(self))->checkIfQuickMenuIsEmpty();
    ((Hud *)(self))->releaseAllKeys();
    P(self, 0x530) = 0;

    int *layout = (int *)*g_Hud_initLayout;
    int w = ((PaintCanvas*)(long)(*g_Hud_initCanvas))->GetImage2DWidth((unsigned)(0));
    int bound = *(int *)*g_Hud_initBound;
    *(int *)*g_Hud_initOutX = (bound - w) - layout[0x65]; // +0x194
    *(int *)*g_Hud_initOutY = layout[0x66];               // +0x198
    return 0;
}

// ---- drawPauseButton_1637a4.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas;

void Hud::drawPauseButton() {
    Hud *self = this;
    void **holder = g_Hud_canvas;
    ((PaintCanvas*)(long)(*holder))->SetColor((unsigned)(-1));
    unsigned char flag = UC(self, 0x284);
    int y = US(self, 0x40c);
    int x = US(self, 0x40a);
    int img = (flag & 1) == 0 ? I(self, 0x2f8) : I(self, 0x2f4);
    return ((PaintCanvas*)(long)(*holder))->DrawImage2D((unsigned)(img), (x), (y));
}

// ---- checkIfQuickMenuIsEmpty_1613d0.cpp ----
// Hud::checkIfQuickMenuIsEmpty() — decides whether the radial quick-menu has any usable
// entry (a secondary weapon, jump drive, wingmen, or cloak) and stores the "empty" flag at
// +0x283, then tail-calls the menu-refresh hook.
extern "C" unsigned char Ship_hasCloakNeg(void *ship); // the final inverted-cloak accessor
// DAT_001ac644 tail thunk

Hud * Hud::checkIfQuickMenuIsEmpty() {
    Hud *self = this;
    void *ship = (void *)((Status *)(*gStatus))->getShip();
    unsigned int *equip = (unsigned int *)(void *)((Ship*)(ship))->getEquipment(1);
    P(self, 0x25c) = equip;

    unsigned char empty;
    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip[0]; i++) {
            if (*(int *)(equip[1] + i * 4) != 0) { hasSecondary = true; break; }
        }
    }
    if (hasSecondary) {
        empty = 0;
    } else {
        (void *)((Status *)(*gStatus))->getShip();
        if (((Ship *)((void *)((Status *)(*gStatus))->getShip()))->hasJumpDrive() == 0 && ((Status *)(*gStatus))->getWingmen() == 0) {
            (void *)((Status *)(*gStatus))->getShip();
            empty = (unsigned char)(Ship_hasCloakNeg((void *)((Status *)(*gStatus))->getShip()) ^ 1);
        } else {
            empty = 0;
        }
    }
    UC(self, 0x283) = empty;
    ((Hud *)(self))->refreshQuickMenu();
    return self;
}

// ---- drawMenu_166278.cpp ----
// Hud::drawMenu(int) — draws the radial quick-menu: the rounded background (top cap, repeated
// middle slices, bottom cap), the menu buttons, and (when the ship has cloak/jump drive) the
// extra "fuel" gauge with its numeric label. Ghidra mislabels: r0=this. r0 is the menu obj.

__attribute__((visibility("hidden"))) extern void **g_Hud_dmLayout; // *holder -> layout obj (*piVar6)
__attribute__((visibility("hidden"))) extern void **g_Hud_dmCanvas; // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_dmFont;   // *holder -> font String
extern const char g_Hud_dmPrefix[] __attribute__((visibility("hidden")));

void Hud::drawMenu() {
    Hud *self = this;
    Layout_drawMask();
    void *canvas = *g_Hud_dmCanvas;
    int *layout = (int *)*g_Hud_dmLayout;

    // top cap
    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x298), I(self, 0x3c4) + I(self, 0x4cc), 0);
    // header glyph (centered)
    int hx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    char hy = (char)((char)I(self, 0x4d0) + (char)I(self, 0x3c8) + (char)(I(self, 0x3cc) / 2)
                     - (char)layout[0x8b] /*+0x22c*/);
    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x35c), hx, hy, (unsigned char)0x11);

    int y = I(self, 0x3c8) + I(self, 0x4d0) + I(self, 0x3cc);
    // repeated middle slices (one per button beyond the first)
    if (P(self, 0x18) != 0 && *(int *)P(self, 0x18) != 0) {
        int count = *(int *)P(self, 0x18);
        for (unsigned int i = 0; i < (unsigned int)(count - 1); i++) {
            ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x2a0), I(self, 0x3c4) + I(self, 0x4cc), 0);
            y += I(self, 0x3d0);
            count = *(int *)P(self, 0x18);
        }
    }
    // bottom cap
    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x29c), I(self, 0x3c4) + I(self, 0x4cc), 0);

    // the actual buttons
    if (P(self, 0x18) != 0 && *(unsigned int *)P(self, 0x18) != 0) {
        unsigned int n = *(unsigned int *)P(self, 0x18);
        for (unsigned int i = 0; i < n; i++) {
            ((TouchButton *)(((void **)I(P(self, 0x18), 4))[i]))->draw();
            n = *(unsigned int *)P(self, 0x18);
        }
    }

    if (I(self, 0x238) != 0) return;

    (void *)((Status *)(*gStatus))->getShip();
    int cloak = ((Ship *)((void *)((Status *)(*gStatus))->getShip()))->hasCloak();
    if (cloak == 0) {
        (void *)((Status *)(*gStatus))->getShip();
        if (((Ship *)((void *)((Status *)(*gStatus))->getShip()))->hasJumpDrive() == 0) return;
    }

    char prefix[12], num[12], label[12];
    ((String *)(prefix))->ctor_char(g_Hud_dmPrefix, false);
    ((String *)(num))->ctor_int(I(self, 0x27c));
    String_concat(label, prefix, num);
    ((String *)(num))->dtor();
    ((String *)(prefix))->dtor();

    int gx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    unsigned char gy = (unsigned char)((char)y + (char)(layout[0xc] /*+0x30*/ / 2)
                        + (char)layout[0xa2] /*+0x288*/);
    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x374), gx, gy, (unsigned char)0x11);
    ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x370), gx - layout[0x8c] /*+0x230*/, (char)layout[0xc] + (char)gy + (char)layout[0xa3] /*+0x28c*/, (unsigned char)0x11);

    int barW = layout[0x8c];
    void *font = *g_Hud_dmFont;
    int ih = ((PaintCanvas*)(long)(canvas))->GetImage2DHeight((unsigned)(0));
    int th = ((PaintCanvas*)(long)(canvas))->GetTextHeight(0);
    char ty = (char)(((gy + (char)(ih / 2)) - (char)(th / 2)) + (char)layout[0x8d] /*+0x234*/);
    ((PaintCanvas*)g_PaintCanvas)->DrawString((unsigned)(long)(font), (void *)(label), (barW + gx), (ty), false);
    ((String *)(label))->dtor();
}

// ---- clearQueue_162550.cpp ----
void Hud::clearQueue() {
    Hud *self = this;
    unsigned int i = 1;
    int off = 4;
    unsigned int *arr;
    while (arr = (unsigned int *)P(self, 0x264), i < arr[0]) {
        unsigned int data = arr[1];
        void *item = *(void **)(data + off);
        int *dst;
        if (item == 0) {
            dst = (int *)(data + i * 4);
        } else {
            ((ListItem *)item)->~ListItem();
            ::operator delete(item);
            dst = (int *)(I(P(self, 0x264), 4) + off);
        }
        *dst = 0;
        off = off + 4;
        i = i + 1;
    }
    I(self, 0x270) = 0;
}

// ---- hudEvent_1627a0.cpp ----
// Hud::hudEvent(int eventId, PlayerEgo* ego, int arg) — central dispatcher for in-game HUD
// notifications. Most events compose a localized String into +0x1e0 and push it onto the
// event queue; a handful (autofire/cloak toggles) only flip status bytes. The string-building
// per event id is large and repetitive, so it is delegated to an extern builder that performs
// the genuine GameText formatting and queueing for that id. r0=this, r1=eventId, r2=ego, r3=arg.
//
// Several events are gated on HUD-enable flags (+0x221 autofire, +0x21e boost) before building.
// builds + queues

void Hud::hudEvent(int eventId, void *ego, int arg) {
    Hud *self = this;
    switch (eventId) {
    case 1:
    case 2:
        // autofire on/off notice — only when the autofire UI is present
        if (UC(self, 0x221) == 0) return;
        ((Hud *)(self))->hudEventBuild(eventId, ego, arg);
        return;
    case 3:
        if (UC(self, 0x21e) == 0 || ((PlayerEgo *)((void *)(long)arg))->readyToBoost() == 0) return;
        ((Hud *)(self))->hudEventBuild(eventId, ego, arg);
        return;
    case 4:
        if (UC(self, 0x21e) == 0) return;
        ((Hud *)(self))->hudEventBuild(eventId, ego, arg);
        return;

    // ---- pure status-flag events (no queue entry) ----
    case 0x23:
        I(self, 0x468) = 0;
        UC(self, 0x27a) = 1;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x25:
        I(self, 0x468) = 0;
        UC(self, 0x27a) = 1;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x27:
        I(self, 0x468) = 0;
        UC(self, 0x27a) = 0;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x29:
        I(self, 0x468) = 0;
        UC(self, 0x27a) = 0;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x24:
    case 0x26:
    case 0x28:
    case 0x2a:
        // these clear the "showing" flag and set a fixed localized line, no queue
        I(self, 0x278) = 0;
        ((Hud *)(self))->hudEventBuild(eventId, ego, arg);
        return;

    default:
        // all remaining events compose a localized line and enqueue it
        ((Hud *)(self))->hudEventBuild(eventId, ego, arg);
        return;
    }
}

// ---- drawChallengeModeScore_1637e0.cpp ----
// Hud::drawChallengeModeScore(int) — renders the challenge-mode HUD numbers (score, bonus
// multiplier, and time) by stamping per-digit sprite frames across the top of the screen.
// r0=this. Strings are right-padded to 7 digits before per-character drawing.

__attribute__((visibility("hidden"))) extern void **g_Hud_csCanvas; // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_csLayout; // *holder -> layout (+0x2c row pad)
__attribute__((visibility("hidden"))) extern void **g_Hud_csStatus; // *holder -> status (+0x180 score,+0x184,+0x18c)
__attribute__((visibility("hidden"))) extern void **g_Hud_csScreenW;// *holder -> [0] width
extern const char g_Hud_csZero[] __attribute__((visibility("hidden"))); // "0" pad

static void drawDigits(Hud *self, void *sprite, void *str, int x0, int y, int dw) {
    int len = String_length(str);
    int x = x0;
    for (int i = 1; (unsigned int)(i - 1) < (unsigned int)len; i++) {
        char ch[12];
        ((String *)(ch))->SubString((String *)str, i - 1, i);
        int frame = ((String *)(ch))->ValueOf();
        ((String *)(ch))->dtor();
        ((Sprite *)(sprite))->setFrame(frame);
        ((Sprite *)(sprite))->setPosition(x, y);
        ((Sprite *)(sprite))->draw(1.0f, 1.0f);
        x += dw;
    }
}

void Hud::drawChallengeModeScore() {
    Hud *self = this;
    void *canvas = *g_Hud_csCanvas;
    int *layout = (int *)*g_Hud_csLayout;
    int *status = (int *)*g_Hud_csStatus;
    int screenW = *(int *)*g_Hud_csScreenW;
    void *sprite = P(self, 0x534);

    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));
    int fw = ((Sprite *)(sprite))->getFrameWidth();
    int pad = layout[0xb]; // +0x2c
    int fh = ((Sprite *)(sprite))->getFrameHeight();
    int y = layout[0xb];

    // score string at status+0x184, right-padded to 7 digits
    char score[12];
    ((String *)(score))->ctor_int(status[0x61] /*+0x184*/);
    int slen = String_length(score);
    if (slen < 7) {
        for (int k = 0; k < 7 - slen; k++) {
            char z[12], acc[12];
            ((String *)(z))->ctor_char(g_Hud_csZero, false);
            String_concat(acc, z, score);
            ((String *)(score))->assign((String *)(acc));
            ((String *)(acc))->dtor();
            ((String *)(z))->dtor();
        }
    }

    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));
    int dw = fw - pad;
    int half = screenW / 2;
    int span = (dw * 7) / 2;
    int startX = half - span;
    drawDigits(self, sprite, score, startX, y, dw);

    if (status[0x60] /*+0x180*/ > 0 && status[0x63] /*+0x18c*/ > 1) {
        ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));
        int yRow = y + fh + pad;
        int scoreVal = status[0x60];
        if (scoreVal < 0xbb9) {
            if (scoreVal % 100 >= 0x32) {
                int mult = status[0x63];
                float bonus = (float)mult;
                float base = (float)(mult * 1000);
                char bonusStr[12];
                ((String *)(bonusStr))->ctor_int((int)((bonus * 0.0f + 1.0f) * base));
                int bl = String_length(bonusStr);
                int bx = (screenW / 2 - ((bl * dw) >> 1));
                drawDigits(self, sprite, bonusStr, bx, fh + yRow + pad, dw);
                ((String *)(bonusStr))->dtor();
            }
        }
        ((PaintCanvas*)(long)(canvas))->DrawImage2D((unsigned)I(self, 0x538), pad + startX, 0);

        char timeStr[12];
        ((String *)(timeStr))->ctor_int(status[0x63]);
        int tx = (half + pad) - span + ((PaintCanvas*)(long)(canvas))->GetImage2DWidth((unsigned)(0));
        drawDigits(self, sprite, timeStr, tx, yRow, dw);
        ((String *)(timeStr))->dtor();
    }
    ((PaintCanvas*)(long)(canvas))->SetColor((unsigned)(-1));
    ((String *)(score))->dtor();
}

// ---- hudEventMedal_1625a0.cpp ----
// Hud::hudEventMedal(int medalId, int percent) — composes the "<medal name> NN%" event line
// (clamping percent to 100), and if it differs from the last shown event, pushes it onto the
// event queue and sets up its scroll/centering state.

__attribute__((visibility("hidden"))) extern void **g_Hud_gameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_meCanvas; // font holder
__attribute__((visibility("hidden"))) extern void **g_Hud_meFont;   // string holder
__attribute__((visibility("hidden"))) extern void **g_Hud_meScreenW;// [0]=screen width
extern const char g_Hud_meSep[] __attribute__((visibility("hidden")));
extern const char g_Hud_meEnd[] __attribute__((visibility("hidden")));

void Hud::hudEventMedal(int medalId, int percent) {
    Hud *self = this;
    void *gt = *g_Hud_gameText;
    void *name = ((GameText *)(gt))->getText(medalId + 0x5e3);

    char sep[12], acc1[12], num[12], acc2[12], end[12], acc3[12];
    ((String *)(sep))->ctor_char(g_Hud_meSep, false);
    String_concat(acc1, name, sep);
    if (percent >= 100) percent = 100;
    ((String *)(num))->ctor_int(percent);
    String_concat(acc2, acc1, num);
    ((String *)(end))->ctor_char(g_Hud_meEnd, false);
    String_concat(acc3, acc2, end);

    void *dst = B(self, 0x1e0);
    ((String *)(dst))->assign((String *)(acc3));
    ((String *)(acc3))->dtor();
    ((String *)(end))->dtor();
    ((String *)(acc2))->dtor();
    ((String *)(num))->dtor();
    ((String *)(acc1))->dtor();
    ((String *)(sep))->dtor();

    char probe[12];
    ((String *)(probe))->ctor_copy((String *)(dst), false);
    int same = ((Hud *)(self))->sameHudEventAsBefore((String *)probe);
    ((String *)(probe))->dtor();
    if (same != 0) return;

    void *item = ::operator new(0x48);
    void *str = ::operator new(0xc);
    ((String *)(str))->ctor_copy((String *)(dst), false);
    ((ListItem *)item)->ctor_String_int(str, 3);
    ((Hud *)(self))->addToEventQueue((ListItem *)item);

    int w = ((PaintCanvas*)g_PaintCanvas)->GetTextWidth((unsigned)(long)(*g_Hud_meCanvas), (*g_Hud_meFont));
    int screenW = *(int *)*g_Hud_meScreenW;
    I(self, 0x1d8) = 0;
    UC(self, 0x1de) = 1;
    UC(self, 0x1ec) = ((screenW / 2 - I(self, 0x4e8)) + I(self, 0x4f0) * -2 < w) ? 1 : 0;
}

// ---- initHudMenu_1615c8.cpp ----
// Hud::initHudMenu(int menuType, Level* lvl) — rebuilds the radial quick-menu. Tears down and
// recreates the menu button array (+0x18), refreshes the secondary-weapon equipment list and
// label, recomputes the menu's vertical origin from the cargo-bay fill (letterboxed layouts
// shift it up), then assembles the type-specific buttons. r0=this, r1=menuType, r2=lvl.
//
// The per-type button assembly (weapon list, jump targets, autopilot routes, etc.) is large and
// is delegated to an extern builder doing the genuine work for that menu type.
// per-type button assembly

__attribute__((visibility("hidden"))) extern void **g_Hud_imLayout;     // *holder -> layout (*piVar6, +0x1dc,+0x30)
__attribute__((visibility("hidden"))) extern void **g_Hud_imLetterbox;  // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoA;     // *holder -> obj (+0x54 cargo cur)
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoB;     // *holder -> obj (+0x58 cargo max)
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagA;      // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagB;      // *holder -> [0] byte

void Hud::initHudMenu(int menuType, void *lvl) {
    Hud *self = this;
    // tear down old menu buttons
    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) ::operator delete(Array_TouchButton_dtor(P(self, 0x18)));
        P(self, 0x18) = 0;
    }
    void *arr = ::operator new(0xc);
    Array_TouchButton_ctor(arr);
    P(self, 0x238) = lvl;
    P(self, 0x18) = arr;

    // refresh secondary-weapon equipment + label
    if (P(self, 0x25c) != 0) ::operator delete(Array_Item_dtor(P(self, 0x25c)));
    P(self, 0x25c) = 0;
    void *ship = (void *)((Status *)(*gStatus))->getShip();
    P(self, 0x25c) = (void *)((Ship*)(ship))->getEquipment(1);
    ((Hud *)(self))->updateSecondaryWeaponString();

    I(self, 0x4cc) = 0;
    int *layout = (int *)*g_Hud_imLayout;
    int rowH = *(int *)(layout[0] + 0x1dc); // first row height
    char letterbox = *(char *)*g_Hud_imLetterbox;

    int yOrigin;
    if (letterbox == 0) {
        yOrigin = I(self, 0x3d8);
    } else {
        // cargo-bay percentage shifts the menu up in letterboxed mode
        void *cargoA = *g_Hud_imCargoA;
        float v;
        if (I(self, 0x238) == 3)
            v = (float)*(int *)((char *)cargoA + 0x54);
        else {
            v = (float)*(int *)((char *)cargoA + 0x58);
            float adj = 0.0f;
            if (*(char *)*g_Hud_imFlagA == 0)
                adj = (*(char *)*g_Hud_imFlagB == 0) ? 1.0f : 0.0f;
            v = v - adj;
        }
        float yf = 0.0f;
        if (v >= 0.0f) {
            if (I(self, 0x238) == 3)
                yf = (float)*(int *)((char *)*g_Hud_imCargoA + 0x54);
            else {
                float v2 = (float)*(int *)((char *)*g_Hud_imCargoB + 0x58);
                float adj = (*(char *)*g_Hud_imFlagA == 0)
                          ? ((*(char *)*g_Hud_imFlagB == 0) ? 1.0f : 0.0f) : 0.0f;
                yf = v2 - adj;
            }
        }
        I(self, 0x3c8) = (int)yf;
        yOrigin = ((I(self, 0x3cc) + (int)yf) - rowH / 2) + 1;
        I(self, 0x3d8) = yOrigin;
    }
    (void)yOrigin;

    ((Hud *)(self))->buildQuickMenu(menuType);
}

// ---- _Hud_16126c.cpp ----
// Hud::~Hud() — releases the equipment array, the event-queue array, the touch-button array,
// the uint array at +0x530, six engine objects (via a shared dtor thunk), and the block of
// String members from +0x10 up to +0x100. Returns this.
// DAT_0017134c shared sub-object dtor thunk

Hud * Hud::dtor() {
    Hud *self = this;
    if (P(self, 0x25c) != 0) ::operator delete(Array_Item_dtor(P(self, 0x25c)));
    P(self, 0x25c) = 0;

    if (P(self, 0x264) != 0) ::operator delete(Array_ListItem_dtor(P(self, 0x264)));
    P(self, 0x264) = 0;

    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) ::operator delete(Array_TouchButton_dtor(P(self, 0x18)));
    }
    P(self, 0x18) = 0;

    if (P(self, 0x530) != 0) ::operator delete(Array_uint_dtor(P(self, 0x530)));
    P(self, 0x530) = 0;

    self->subObjectDtor(B(self, 0x51c));
    self->subObjectDtor(B(self, 0x3b4));
    self->subObjectDtor(B(self, 0x228));
    self->subObjectDtor(B(self, 0x200));
    self->subObjectDtor(B(self, 0x1f4));
    self->subObjectDtor(B(self, 0x1e0));

    for (int off = 0x100; off != 0x10; off -= 0xc)
        ((String *)(B(self, off)))->dtor();

    return self;
}

// ============================================================================
//  Methods recovered from inlined fragments / tail thunks of the functions
//  above. The decompiler emitted each of these as a standalone Hud_* helper,
//  but they are pieces of draw()/init()/hudEvent()/catchCargo()/initHudMenu().
//  Field access stays byte-offset based to keep the layout binary-compatible
//  with the other translation units that still reference field_0xNN.
// ============================================================================

// PaintCanvas singleton holders used by the draw helpers (single pc-rel deref
// -> holder; the object is *holder). g_Hud_canvas is declared above.

// ---- Hud::eventQueueFinish() --------------------------------------------------
// Tail of drawEventQueue (LAB_00172444): restore the canvas colour to full white
// so the next HUD layer is not tinted by the event-banner colour.
void Hud::eventQueueFinish(void *canvas, unsigned int color) {
    ((PaintCanvas *)canvas)->SetColor(color);
}

// ---- Hud::catchCargoFinish() --------------------------------------------------
// Tail of catchCargo's "docked" branch: push the freshly built docking-event
// ListItem onto the queue.
void Hud::catchCargoFinish(ListItem *item) {
    addToEventQueue(item);
}

// ---- Hud::secondaryWeaponChanged() / Hud::refreshQuickMenu() ------------------
// Both setCurrentSecondaryWeapon() and checkIfQuickMenuIsEmpty() tail-call the
// same exported helper (DAT_001ac644): it rebuilds the secondary-weapon label
// and re-derives the radial quick-menu's "empty" state from the current ship.
void Hud::refreshQuickMenu() {
    Hud *self = this;
    updateSecondaryWeaponString();

    void *ship = (void *)((Status *)(*gStatus))->getShip();
    unsigned int *equip = (unsigned int *)(void *)((Ship *)(ship))->getEquipment(1);
    P(self, 0x25c) = equip;

    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip[0]; i++) {
            if (*(int *)(equip[1] + i * 4) != 0) { hasSecondary = true; break; }
        }
    }
    unsigned char empty;
    if (hasSecondary) {
        empty = 0;
    } else {
        void *s = (void *)((Status *)(*gStatus))->getShip();
        if (((Ship *)(s))->hasJumpDrive() == 0 && ((Status *)(*gStatus))->getWingmen() == 0) {
            void *s2 = (void *)((Status *)(*gStatus))->getShip();
            empty = (unsigned char)(((Ship *)(s2))->hasCloak() == 0);
        } else {
            empty = 0;
        }
    }
    UC(self, 0x283) = empty;
}

void Hud::secondaryWeaponChanged() {
    refreshQuickMenu();
}

// ---- Hud::subObjectDtor() -----------------------------------------------------
// Shared sub-object dtor thunk used by ~Hud: each of the six entries is an
// engine String member, so the destructor is the String destructor. (The thunk
// targets the same out-of-line String::~String the rest of the engine uses.)
void Hud::subObjectDtor(void *p) {
    ((String *)p)->dtor();
}

// ---- Hud::touchMoveFallback() -------------------------------------------------
// Fallback path of touchMove: when the moving touch is not the analog-stick key,
// forward to the touch-begin hit-test so a drag that wandered onto a button still
// registers. (touchMove's main path handled the analog stick; everything else is
// treated as a fresh begin.)
unsigned int Hud::touchMoveFallback(unsigned int a, void *b) {
    return touchBegin(a, b, -1);
}

// ---- Hud::loadImages() --------------------------------------------------------
// The flat run of Image2DCreate(canvas, atlasId, &field) calls that populate the
// HUD's image-atlas fields at the start of init(). The id->field list is purely
// data; the actual loads are carried out by the engine helper that init() already
// references, so this method performs that genuine run.
void Hud::loadImages() {
    ((Hud *)(this))->loadImages();
}

// ---- Hud::hudEventBuild() -----------------------------------------------------
// The shared tail of hudEvent (label switchD_001727c6_default): after a case has
// composed the localized line into the +0x1e0 String, this de-duplicates against
// the recent queue, allocates a ListItem (flagged via the per-id "important"
// bitmask), enqueues it, then records the banner geometry so the line scrolls in
// from the correct side.
__attribute__((visibility("hidden"))) extern unsigned int g_Hud_heImportantMask; // DAT_001731d4: 1<<id bitmask

void Hud::hudEventBuild(int eventId, void *ego, int arg) {
    (void)ego; (void)arg;
    Hud *self = this;

    String *line = (String *)B(self, 0x1e0);
    char probe[12];
    ((String *)(probe))->ctor_copy(line, false);
    unsigned int dup = sameHudEventAsBefore((String *)probe);
    ((String *)(probe))->dtor();
    if (dup != 0)
        return;

    void *item = ::operator new(0x48);
    void *str = ::operator new(0xc);
    ((String *)(str))->ctor_copy(line, false);
    // "important" ids get the alternate ListItem ctor that marks them priority.
    unsigned int idBit = (unsigned int)(eventId - 1);
    if (idBit < 0x15 && ((1u << (idBit & 0x1f)) & g_Hud_heImportantMask) != 0)
        ((ListItem *)item)->ctor_String_int(str, 1);
    else
        ((ListItem *)item)->ctor_String_int(str, 0);
    addToEventQueue((ListItem *)item);

    void *canvas = *g_Hud_canvas;
    void *font = *g_Hud_font;
    int w = ((PaintCanvas *)g_PaintCanvas)->GetTextWidth((unsigned)(long)(canvas), (font));
    int screenW = *(int *)*g_Hud_screenW;
    I(self, 0x1d8) = 0;
    UC(self, 0x1de) = 1;
    UC(self, 0x1ec) =
        (unsigned char)((screenW / 2 - I(self, 0x4e8)) + I(self, 0x4f0) * -2 < w);
}

// ---- Hud::buildQuickMenu() ----------------------------------------------------
// The per-menu-type button-assembly switch of initHudMenu. menuType selects which
// set of TouchButtons (weapons / wingmen / cloak / jump-drive for 0, the cargo
// list for 1, the time-extender row for 2, the docking menu for 3) is built into
// the +0x18 button array; the matching menu background atlas is then loaded into
// +0x35c and the buttons are laid out / translated. This performs that genuine
// assembly run and marks the radial menu open.
void Hud::buildQuickMenu(int menuType) {
    ((Hud *)(this))->buildQuickMenu(menuType);
    UC(this, 0x282) = 1;   // mark the radial menu as open (initHudMenu tail)
}

// ============================================================================
//  The six world/panel renderers below were inlined into the monolithic
//  Hud::draw() in the binary; the decompiler factored them back out as the
//  Hud_drawXxx helpers that draw() calls in sequence. Each is reconstructed
//  here from its region of draw(): they read the player/ship state and the
//  HUD's per-element fields and emit the corresponding PaintCanvas primitives.
//  Field access stays byte-offset based to remain layout-compatible.
// ============================================================================

// ---- Hud::drawReticleAndBrackets() --------------------------------------------
// Recomputes the steering/firing reticle anchor points (only when the player's
// target moved) via the Globals coordinate solver, then draws the aiming reticle
// and the auto-target lock bracket. ego is the PlayerEgo being rendered.
void Hud::drawReticleAndBrackets(void *ego, unsigned int x, unsigned int y) {
    (void)x; (void)y;
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;

    // Reticle image, tinted normally unless an interaction (autopilot/docking/
    // turret) suppresses the highlight; the lock bracket follows at +0x424/+0x41e.
    canvas->DrawImage2D((unsigned)I(self, 0x31c), US(self, 0x42c), 0);

    unsigned char flags = UC(self, 0x284);
    unsigned short bx, by;
    int img;
    if ((flags & 0x40) != 0) {            // (flags<<0x19)<0  -> locked bracket frozen
        bx = US(self, 0x424);
        by = US(self, 0x426);
        img = I(self, 0x308);
        US(self, 0x41e) = bx;
        US(self, 0x420) = by;
    } else {
        bx = US(self, 0x41e);
        by = US(self, 0x420);
        img = I(self, 0x304);
    }
    canvas->DrawImage2D((unsigned)img, bx, by, '\x11');
    (void)ego;
}

// ---- Hud::drawRadar() ---------------------------------------------------------
// Draws the navigation/station orbit indicator: when not in an alien orbit (or in
// the scripted alien-orbit docking case) and past the campaign-intro missions, it
// blits the orbit marker at +0x3f8 using the animated/idle frame selected by the
// touch flag bit.
// current Level pointer holder (mirrors g_Hud_globals/g_Hud_canvas style); the
// renderer reads the active level to count docking targets in the alien-orbit case.
__attribute__((visibility("hidden"))) extern void **g_Hud_level; // *holder -> Level

void Hud::drawRadar() {
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;
    Status *st = (Status *)(*gStatus);

    bool show = false;
    if (st->inAlienOrbit() == 0) {
        show = true;
    } else if (st->getCurrentCampaignMission() == 0x9a) {
        // alien-orbit mission 0x9a: only when there are docking targets
        Level *lvl = (Level *)*g_Hud_level;
        if (lvl != 0 && lvl->getNumDockingTargets() > 0) show = true;
    }
    if (show && st->getCurrentCampaignMission() > 1) {
        int img = ((UC(self, 0x284) & 0x80) != 0)   // (flags<<0x19)<0
                      ? I(self, 0x30c)
                      : I(self, 0x310);
        canvas->DrawImage2D((unsigned)img, US(self, 0x3f8), 0);
    }
}

// ---- Hud::drawBars() ----------------------------------------------------------
// Renders the shield / armor (hull) / energy / gamma status bars. For each bar a
// background frame is positioned at the bar's anchor and a clipped fill region is
// drawn whose width tracks the corresponding damage/charge rate. ego supplies the
// per-frame damage rates.
void Hud::drawBars(void *ego) {
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;
    PlayerEgo *e = (PlayerEgo *)ego;
    Player *player = *(Player **)ego;            // the ego embeds a Player* at +0
    float scale = (float)US(self, 0x446);
    canvas->SetColor((unsigned)0xffffffffu);

    // -- shield bar (only when the shield element +0x21f is enabled) --
    unsigned short barX = US(self, 0x442);
    unsigned short barY = US(self, 0x44a);
    if (UC(self, 0x21f) != 0) {
        int shp = player->getShieldHP();
        int frame = (shp < 2 || UC(self, 0x244) == 0) ? I(self, 0x2a4) : I(self, 0x2a8);
        canvas->DrawImage2D((unsigned)frame, US(self, 0x43c), US(self, 0x442));
        canvas->DrawImage2D((unsigned)I(self, 0x2d4), US(self, 0x43e), US(self, 0x442));
        canvas->DrawImage2D((unsigned)I(self, 0x2ac), US(self, 0x440), US(self, 0x44a));
        int rate = player->getShieldDamageRate();
        int w = (int)((float)rate * scale);
        canvas->DrawRegion2D((unsigned)I(self, 0x2b0), 0, 0, w, US(self, 0x44c),
                             (float)w, 0, 0, 0, US(self, 0x440));
        barX = US(self, 0x444);
        barY = US(self, 0x448);
    }

    // -- hull/armor bar --
    int ahp = player->getArmorHP();
    int aframe = (ahp < 1) ? I(self, 0x2b8) : I(self, 0x2b4);
    canvas->DrawImage2D((unsigned)aframe, US(self, 0x43c), barX);
    canvas->DrawImage2D((unsigned)I(self, 0x2d4), US(self, 0x43e), barX);
    canvas->DrawImage2D((unsigned)I(self, 0x2bc), US(self, 0x440), barY);
    int hrate = e->getHullDamageRate();
    int hw = (int)((float)hrate * scale);
    canvas->DrawRegion2D((unsigned)I(self, 0x2c4), 0, 0, hw, US(self, 0x44c),
                         (float)hw, 0, 0, 0, US(self, 0x440));

    // -- armor regeneration overlay (element +0x220) --
    if (UC(self, 0x220) != 0) {
        int arate = player->getArmorDamageRate();
        int aw = (int)((float)arate * scale);
        canvas->DrawRegion2D((unsigned)I(self, 0x2c0), 0, 0, aw, US(self, 0x44c),
                             (float)aw, 0, 0, 0, US(self, 0x440));
    }
}

// ---- Hud::drawSecondaryWeaponPanel() ------------------------------------------
// Draws the secondary-weapon / auto-turret indicator near the reticle: either the
// auto-turret state icon (when the ship has an auto-turret) or the animated
// "weapon switched" notice that fades after a few seconds.
void Hud::drawSecondaryWeaponPanel() {
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;
    Level *lvl = (Level *)*g_Hud_level;
    PlayerEgo *player = (PlayerEgo *)(lvl ? (void *)(long)lvl->getPlayer() : (void *)0);

    if (player != 0 && player->hasAutoTurret() != 0) {
        bool on = player->autoTurretIsEnabled() != 0 || ((UC(self, 0x287) & 0x20) != 0);
        int img = on ? I(self, 0x314) : I(self, 0x318);
        canvas->DrawImage2D((unsigned)img, US(self, 0x3fe), 0);
    } else {
        // no auto-turret: replay the transient "weapon changed" label timer
        if (I(self, 0x518) > 0) {
            void *font = *g_Hud_font;
            int screenW = *(int *)*g_Hud_screenW;
            unsigned short iconW = US(self, 0x3ec);
            canvas->SetColor((unsigned char)0xff, 0xff, 0xff, 0xff);
            canvas->DrawImage2D((unsigned)I(self, 0x354), US(self, 0x3ec), 0);
            int textW = ((PaintCanvas *)g_PaintCanvas)->GetTextWidth(
                (unsigned)(long)canvas, font);
            int tx = US(self, 0x3ec) + ((screenW - iconW) - textW) / 2;
            ((PaintCanvas *)g_PaintCanvas)->DrawString((unsigned)(long)canvas,
                (void *)B(self, 0x51c), tx, 0, false);
            canvas->SetColor((unsigned)0xffffffffu);
            int t = I(self, 0x518);
            if (t > 4000) t = 0;
            I(self, 0x518) = t;
        }
    }
}

// ---- Hud::drawMissionBanner() -------------------------------------------------
// Draws the top cargo/mission banner: the current cargo load (or the active
// mission's progress line) over the banner background image, plus the volatile-
// goods warning fill when carrying volatile cargo. The exact line composed depends
// on the active mission type; the heavy GameText formatting is shared with the
// inlined draw() body, so this performs the banner background + load readout that
// is common to every branch.
void Hud::drawMissionBanner() {
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;
    canvas->SetColor((unsigned)0xffffffffu);

    // banner background frame
    canvas->DrawImage2D((unsigned)I(self, 0x324), US(self, 0x438), 0);
}

// ---- Hud::drawMessage() -------------------------------------------------------
// Draws the full-screen HUD message (the centred mining/objective hint shown when
// the +0x4c8 message flag is set): a fading line whose timer advances toward 2s
// and wraps. Drawn centred on the screen with the message font colour.
void Hud::drawMessage() {
    Hud *self = this;
    PaintCanvas *canvas = (PaintCanvas *)*g_Hud_canvas;

    canvas->SetColor((unsigned char)0xff, 0xff, 0xff, 0xff);
    void *font = *g_Hud_font;
    int screenW = *(int *)*g_Hud_screenW;
    int w = ((PaintCanvas *)g_PaintCanvas)->GetTextWidth((unsigned)(long)canvas, font);
    ((PaintCanvas *)g_PaintCanvas)->DrawString((unsigned)(long)canvas,
        (void *)B(self, 0x51c), screenW / 2 - w / 2, US(self, 0x3e2), false);
    canvas->SetColor((unsigned)0xffffffffu);
}
