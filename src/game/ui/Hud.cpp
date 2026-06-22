#include "game/ui/Hud.h"
#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "engine/render/Sprite.h"
#include "engine/core/GameText.h"
#include "game/core/Globals.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/SolarSystem.h"
#include "game/world/Station.h"
#include "game/ui/TouchButton.h"
#include "game/ui/ListItem.h"
#include "game/mission/Status.h"
#include "game/ship/Ship.h"
#include "game/world/Level.h"
#include "game/ship/Player.h"
#include "engine/render/PaintCanvas.h"
#include "game/ui/Layout.h"

extern void Status_replaceHash(void *out, void *tmpl, void *a, void *b, void *c);

void Image2DCreate(void *canvas, unsigned short id, void *outField);

extern void Hud_loadImages(Hud * self);

extern void Hud_buildQuickMenu(Hud *self, int menuType);

#define HUD_INLINE static inline __attribute__((always_inline))
HUD_INLINE
void refreshQuickMenu(Hud * self);
HUD_INLINE

void hudEventBuild(Hud *self, int eventId, void *ego, int arg);

static void buildQuickMenu(Hud *self, int menuType);

static void drawReticleAndBrackets(Hud *self, void *ego, unsigned int x, unsigned int y);

static void drawRadar(Hud * self);

static void drawBars(Hud *self, void *ego);

static void drawSecondaryWeaponPanel(Hud * self);
static void drawMissionBanner(Hud * self);
static void drawMessage(Hud * self);
HUD_INLINE
void drawChallengeModeScoreImpl(Hud * self);
HUD_INLINE

void drawDigits(Hud *self, void *sprite, void *str, int x0, int y, int dw);

void drawControlsInterface(long long t0, long long t1, PlayerEgo *ego, bool letterbox,
                           unsigned int x, unsigned int y) {
    (void) t0;
    (void) t1;
    (void) ego;
    (void) letterbox;
    (void) x;
    (void) y;
}

void Hud::enableFireForTutorial(bool value) {
    this->fireForTutorial = value;
}

void Hud::setVisible(bool value) {
    this->visible = value;
}

void Hud::drawCredits() {
}

void Hud::drawBigNumber(int x, int y, int value, bool flag) {
    (void) x;
    (void) y;
    (void) value;
    (void) flag;
}

int Hud::hudAction(int action, Level *lvl, Radar *radar) {
    (void) action;
    (void) lvl;
    (void) radar;
    return 0;
}

void Hud::setTimeExtender(bool p1, bool p2, bool p3, bool p4) {
    this->field_0x0 = p1;
    this->field_0x280 = p3;
    this->field_0x281 = p4;
    if (p2 && p3) {
        this->timeExtenderDuration = 0x7d0;
        this->timeExtenderTimer = 0x50;
    }
}

void Hud::playerHit() {
    this->shieldHitFlash = 1;
    this->hitFlashTimer = 0;
}

void Hud::addToEventQueue(ListItem *item) {
    Array<ListItem *> *q = this->eventQueue;
    unsigned int idx = 0;
    do {
        unsigned int next = idx + 1;
        if (next >= q->size())
            return;
        idx = next;
    } while ((*q)[idx] != 0);
    (*q)[idx] = item;
    this->eventQueueDirty = 1;
}

unsigned int Hud::firePressed() {
    return (this->touchFlags >> 4) & 1;
}

void Hud::resetAnalogStick() {
    this->lockBracketX = this->reticleX;
}

float Hud::getAnalogY() {
    float num = (float) ((int) this->lockBracketY - (int) this->reticleY);
    float den = (float) this->analogStickRadius;
    return num / den;
}

uint8_t Hud::cargoFull() {
    return this->cargoFullFlag;
}

unsigned int Hud::touchEnd(unsigned int a, unsigned int b, void *key) {
    int i = 0;
    unsigned int ret = 0;
    for (; i != 0x19; i = i + 1) {
        if ((*this->keyArray)[i] == key) {
            ret = (unsigned int) this->elementBits[i];
            this->touchFlags = this->touchFlags & ~ret;
            (*this->keyArray)[i] = 0;
            this->elementBits[i] = 0;
        }
    }
    if (this->quickMenuOpen != 0) {
        Array<TouchButton *> *btns = this->menuButtons;
        if (btns != 0) {
            for (unsigned int j = 0; j < btns->size(); j = j + 1) {
                (*btns)[j]->OnTouchEnd((int) a, (int) b);
                btns = this->menuButtons;
            }
        }
    }
    return ret;
}

void Hud::releaseAllKeys() {
    this->touchFlags = 0;
    for (int i = 0; i != 0x19; i++) {
        Array<void *> *p = this->keyArray;
        if (p != 0) {
            if ((*p)[i] != 0)
                (*p)[i] = 0;
        }
        int *q = this->elementBits;
        q[i] = 0;
    }
    this->field_0x288 = 0;
}

void Hud::closeHudMenu() {
    if (this->menuButtons != 0) {
        for (TouchButton *b: *this->menuButtons) delete b;
        this->menuButtons->clear();
        delete this->menuButtons;
        this->menuButtons = 0;
    }
    this->quickMenuOpen = 0;
}

float Hud::getAnalogX() {
    float num = (float) ((int) this->lockBracketX - (int) this->reticleX);
    float den = (float) this->analogStickRadius;
    return num / den;
}

void Hud::setAutofireEnabled(bool value) {
    this->autofireEnabled = value;
}

uint8_t Hud::isHackingGameActive() {
    return this->hackingGameActive;
}

void Hud::setHackingGameActive(bool value) {
    this->hackingGameActive = value;
}

void Hud::setJumpMapSelected(bool value) {
    this->jumpMapSelectedFlag = value;
}

uint8_t Hud::jumpMapSelected() {
    return this->jumpMapSelectedFlag;
}

void Hud::draw(long long t0, long long t1, PlayerEgo *ego, bool letterbox, unsigned int x, unsigned int y) {
    (void) t0;
    (void) t1;

    // background/letterbox state recorded for the panel renderers
    this->letterbox = (unsigned char) letterbox;

    // world-space HUD elements (reticle, target brackets, radar)
    drawReticleAndBrackets(this, ego, x, y);
    drawRadar(this);

    // player status (shield / armor / energy bars + secondary weapon)
    drawBars(this, ego);
    drawSecondaryWeaponPanel(this);

    // contextual banners
    drawOrbitInformation();
    drawMissionBanner(this);

    // transient event lines
    drawEventQueue();

    // the radial quick-menu, when open
    if (this->quickMenuOpen != 0)
        drawMenu(0);

    // challenge-mode score/time readout
    if (gStatus != nullptr && gStatus->inSupernovaSystem() != 0)
        drawChallengeModeScoreImpl(this);

    // pause button and any pending full-screen message
    drawPauseButton();
    if (this->messageActive != 0)
        drawMessage(this);
}

void Hud::updateQueue(int dt) {
    int t = this->eventQueueTimer + dt;
    this->eventQueueTimer = t;
    int v;
    if (t >= 0xfa1) {
        this->eventQueueTimer = 0;
        Array<ListItem *> *q = this->eventQueue;
        delete (*q)[0];
        (*q)[0] = 0;
        unsigned int i = 0;
        while (true) {
            if (q->size() <= i + 1)
                break;
            (*q)[i] = (*q)[i + 1];
            i = i + 1;
        }
        if ((*q)[1] == 0) {
            this->eventQueueDirty = 0;
        }
        v = 0;
    } else {
        if (t < 0x7d1)
            return;
        if (this->eventQueuePaused != 0)
            return;
        v = 1;
    }
    this->eventQueuePaused = v;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_oiLayout; // *holder -> layout (+0x21c..+0x228)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_oiStatus; // *holder -> status (+0x114)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_oiFont; // *holder -> font String
__attribute__ ((visibility
(
"hidden"
)
)
)
extern GameText **g_Hud_oiGameText; // *holder -> GameText
extern const char g_Hud_oiSep[] __attribute__((visibility("hidden")));
extern const unsigned char g_Hud_secColors[] __attribute__((visibility("hidden"))); // 4 x 12-byte RGB rows

void Hud::drawOrbitInformation() {
    if (gStatus->inAlienOrbit() != 0) return;

    int *layout = (int *) *g_Hud_oiLayout;
    gCanvas->SetColor((unsigned) (-1));
    int x = gCanvas->GetImage2DWidth((unsigned) (0)) + layout[0x87];

    if (((SolarSystem *) (((void *) (long) gStatus->getSystem())))->hasNoOwner() == 0)
        gCanvas->DrawImage2D((unsigned) this->factionLogoImage, 3, 0);

    void *font = *g_Hud_oiFont;
    // station name
    {
        char name[12];
        ((Station *) (name))->getName();
        gCanvas->DrawString((unsigned) (long) (font), *(String *) (name), (x), (char) layout[0x88], false);
        ((String *) (name))->clear();
    }
    gCanvas->SetColor((unsigned) (-1));

    if (gStatus->getCurrentCampaignMission() <= 0xf) return;

    void *sys = ((void *) (long) gStatus->getSystem());
    int sec = ((SolarSystem *) (sys))->getSecurityLevel();
    int idx = ((SolarSystem *) (((void *) (long) gStatus->getSystem())))->getIndex();
    int *status = (int *) *g_Hud_oiStatus;
    if (idx == 0x1a && status[0x45] > 1) sec = 3;

    // system name + separator
    {
        char sysName[12], copy[12], sep[12], acc[12], full[12];
        ((SolarSystem *) (sysName))->getName();
        ((String *) (copy))->ctor_copy((String *) (sysName), false);
        ((String *) (sep))->ctor_char(g_Hud_oiSep, false);
        *(String *) acc = *(String *) copy + *(String *) sep;
        void *txt = (*g_Hud_oiGameText)->getText(0); // id resolved by table
        *(String *) full = *(String *) acc + *(String *) txt;
        gCanvas->DrawString((unsigned) (long) (font), *(String *) (full), (x), (char) layout[0x89], false);
        ((String *) (full))->clear();
        ((String *) (acc))->clear();
        ((String *) (sep))->clear();
        ((String *) (copy))->clear();
        ((String *) (sysName))->clear();
    }

    const unsigned char *row = g_Hud_secColors + sec * 0xc;
    gCanvas->SetColor((unsigned char) (row[0]), (unsigned char) (row[4]), (unsigned char) (row[8]),
                      (unsigned char) (0xff));
    void *secTxt = (*g_Hud_oiGameText)->getText(sec);
    gCanvas->DrawString((unsigned) (long) (font), *(String *) (secTxt), (x), (char) layout[0x8a], false);
}

unsigned int Hud::touchMove(unsigned int a, unsigned int b, void *key) {
    unsigned int i = 0;
    for (; i <= 0x18; i = i + 1) {
        if ((*this->keyArray)[i] == key && this->elementBits[i] == 0x20)
            goto found;
    }
    // not the analog-stick key: treat the drag as a fresh touch-begin so a
    // wandering touch that lands on a button still registers.
    return touchBegin(a, (unsigned int) -1, key);
found:
    int dx = (int) a - (int) this->reticleX;
    int dy = (int) b - (int) this->reticleY;
    float f = (float) (dy * dy + dx * dx);
    float r = gGlobals->sqrt(f);
    int denom = this->analogStickRadius;
    int len = (int) r;
    if (denom < len) {
        short s = (short) (denom * dx / len);
        short base = this->reticleY;
        this->lockBracketX = s + this->reticleX;
        s = (short) (denom * dy / len);
        this->lockBracketY = s + base;
    } else {
        this->lockBracketX = (short) a;
        this->lockBracketY = (short) b;
    }
    return 0x20;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_teCinematic; // *holder -> [0] byte
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_teScreenW; // *holder -> [0] width
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_teScreenH; // *holder -> [0] height

static inline bool span(unsigned short o, int w, unsigned int v) {
    return o <= v && v <= (unsigned int) o + w;
}

static inline bool cspan(unsigned short o, int w, unsigned int v) {
    return (unsigned int) o - w <= v && v <= (unsigned int) o + w;
}

unsigned int Hud::touchedElement(unsigned int x, unsigned int y) {
    Array<TouchButton *> *menu = this->menuButtons;
    if (this->quickMenuOpen != 0 && menu != 0) {
        // quick-menu open: delegate to its buttons
        for (unsigned int i = 0; i < menu->size(); i++) {
            if ((*menu)[i]->OnTouchBegin((int) x, (int) y) != 0)
                return *(unsigned int *) (*this->menuButtons)[i];
            menu = this->menuButtons;
        }
        return 0;
    }

    int w = this->touchHalfExtent;
    int w2 = this->touchHalfExtentSmall;

    bool cinematic = *(char *) *g_Hud_teCinematic != 0;

    if (cinematic) {
        if (span(this->field_0x40a, w, x) && span(this->field_0x40c, w, y)) return 1;
        if (this->hasBoostButton != 0 && span(this->field_0x410, w, x) && span(this->field_0x412, w, y)) return 2;
        if (span(this->field_0x3f8, w, x) && span(this->field_0x3fa, w, y)) return 0x40;
        if (span(this->field_0x404, w, x) && span(this->field_0x406, w, y)) return 0x100;
        if (cspan(this->reticleX, w, x) && cspan(this->reticleY, w2, y)) return 0x20;
        if (span(this->field_0x3f2, w, x) && span(this->field_0x3f4, w, y)) {
            this->field_0x470 = 1000;
            return 0x80;
        }
        if (cspan(this->field_0x3ec, w, x) && span(this->field_0x3ee, w, y)) return 8;
        if (cspan(this->field_0x3e4, w2 >> 1, x) && cspan(this->field_0x3e6, w2 >> 1, y)) return 0x10;
        if (this->quickMenuEmpty == 0 && span(this->field_0x416, this->field_0x41a, x) &&
            span(this->field_0x418, this->field_0x41c, y))
            return 4;
        if (span(this->field_0x3fe, w, x) && span(this->field_0x400, w, y)) return 0x20000000;
        if (this->hackingGameActive != 0) {
            if (span(this->field_0x454, w, x) && span(this->field_0x456, w, y)) return 0x200;
            if (span(this->field_0x458, w, x) && span(this->field_0x45a, w, y)) return 0x400;
            if (span(this->field_0x45e, w, x) && span(this->field_0x460, w, y)) return 0x800;
        }
        return 0;
    }

    // normal layout: extra weapon-select buttons first
    if (this->hackingGameActive != 0) {
        if (span(this->field_0x454, w, x) && span(this->field_0x456, w, y)) return 0x200;
        if (span(this->field_0x458, w, x) && span(this->field_0x45a, w, y)) return 0x400;
        if (span(this->field_0x45e, w, x) && span(this->field_0x460, w, y)) return 0x800;
    }

    int screenW = *(int *) *g_Hud_teScreenW;
    int screenH = *(int *) *g_Hud_teScreenH;

    if (y < (unsigned int) (screenH >> 2)) {
        if (span(this->field_0x40a, w, x) && span(this->field_0x40c, w, y)) return 1;
    } else if (x < (unsigned int) (screenW >> 1)) {
        if (this->hasBoostButton != 0 && cspan(this->field_0x410, w, x) && span(this->field_0x412, w, y)) return 2;
        if (span(this->field_0x3f8, w, x) && span(this->field_0x3fa, w, y)) return 0x40;
        if (span(this->field_0x404, w, x) && span(this->field_0x406, w, y)) return 0x100;
        if (cspan(this->reticleX, w, x) && cspan(this->reticleY, w2, y)) return 0x20;
    } else {
        if (span(this->field_0x3f2, w, x) && span(this->field_0x3f4, w, y)) {
            this->field_0x470 = 1000;
            return 0x80;
        }
        if (cspan(this->field_0x3ec, w, x) && span(this->field_0x3ee, w, y)) return 8;
        if (span(this->field_0x3e4, w2, x) && span(this->field_0x3e6, w2, y)) return 0x10;
        if (this->quickMenuEmpty == 0 && span(this->field_0x416, this->field_0x41a, x) &&
            span(this->field_0x418, this->field_0x41c, y))
            return 4;
        if (span(this->field_0x3fe, w, x) && span(this->field_0x400, w, y)) return 0x20000000;
    }
    return 0;
}

Hud::Hud() {
    // The scratch and labelled String members are default-constructed with the
    // object; init() wires up the rest of the HUD state.
    init();
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern GameText **g_Hud_ccGameText;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_ccTemplate; // *holder -> base format String
extern const char g_Hud_ccHashX[] __attribute__((visibility("hidden")));
extern const char g_Hud_ccHashN[] __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit[] __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit2[] __attribute__((visibility("hidden")));

void Hud::catchCargo(int itemId, int count, bool single, bool missionDelivery, bool extender,
                     bool slotMode, bool aggregate) {
    this->field_0x1d0 = 0;
    this->cargoFullFlag = single ? 1 : 0;

    if (missionDelivery) {
        // mission-delivery line: GameText(0x219) run through two replaceHash passes that
        // substitute the mission-type word and the count into the localized template.
        GameText *gt = *g_Hud_ccGameText;
        void *base = gt->getText(0x219);
        void *dst = &this->field_0x1f4;
        ((String *) (dst))->assign((String *) (base));

        void *tmpl = *g_Hud_ccTemplate;
        char a40[12];
        ((String *) (a40))->ctor_copy((String *) (dst), false);
        int type = gStatus->getMission()->getType();
        void *typeTxt = gt->getText(type == 3 ? 0x56e : 0x56f);
        char a4c[12];
        ((String *) (a4c))->ctor_copy((String *) (typeTxt), false);
        char a58[12];
        ((String *) (a58))->ctor_char(g_Hud_ccHashX, false);
        char out1[12];
        Status_replaceHash(out1, tmpl, a40, a4c, a58);
        ((String *) (dst))->assign((String *) (out1));
        ((String *) (out1))->clear();
        ((String *) (a58))->clear();
        ((String *) (a4c))->clear();
        ((String *) (a40))->clear();

        tmpl = *g_Hud_ccTemplate;
        char a64[12];
        ((String *) (a64))->ctor_copy((String *) (dst), false);
        char a70[12];
        ((String *) (a70))->ctor_int(1);
        char a7c[12];
        ((String *) (a7c))->ctor_char(g_Hud_ccHashN, false);
        char out2[12];
        Status_replaceHash(out2, tmpl, a64, a70, a7c);
        ((String *) (dst))->assign((String *) (out2));
        ((String *) (out2))->clear();
        ((String *) (a7c))->clear();
        ((String *) (a70))->clear();
        ((String *) (a64))->clear();

        String *str = new String(*(String *) dst);
        ListItem *item = new ListItem(str);
        item->field_0x2c = itemId;
        addToEventQueue(item);
        return;
    }

    if (single) {
        // "cargo full" notice: GameText(0x142) pushed as its own event line
        GameText *gt = *g_Hud_ccGameText;
        void *txt = gt->getText(0x142);
        ((String *) (&this->field_0x1f4))->assign((String *) (txt));
        String *str = new String(this->field_0x1f4);
        ListItem *item = new ListItem(str, 1);
        addToEventQueue(item);
        return;
    }

    if (count < 1) return; // nothing to add to the running "+N" tally

    GameText *gt = *g_Hud_ccGameText;

    // aggregate into the previous "+N <unit>" event when one is still showing
    if (aggregate && this->eventQueueDirty != 0) {
        char a0[12];
        ((String *) (a0))->ctor_int(this->cargoAggregateCount);
        char ac[12];
        ((String *) (ac))->ctor_char(g_Hud_ccUnit, false);
        char a94[12];
        *(String *) a94 = *(String *) a0 + *(String *) ac;
        char a88[12];
        ((String *) (a88))->ctor_copy((String *) (a94), false);
        void *unit = gt->getText(itemId + 0x4fa);
        char k34[12];
        *(String *) k34 = *(String *) a88 + *(String *) unit;
        ((String *) (a88))->clear();
        ((String *) (a94))->clear();
        ((String *) (ac))->clear();
        ((String *) (a0))->clear();

        char b8[12];
        ((String *) (b8))->ctor_copy((String *) (k34), false);
        int idx = sameHudEventAsBeforeAggregate(*(String *) b8);
        ((String *) (b8))->clear();
        if (idx >= 0) {
            this->eventQueueTimer = 2000;
            this->cargoAggregateCount += count;
            char nAc[12];
            ((String *) (nAc))->ctor_int(this->cargoAggregateCount);
            char nC4[12];
            ((String *) (nC4))->ctor_char(g_Hud_ccUnit2, false);
            char nA0[12];
            *(String *) nA0 = *(String *) nAc + *(String *) nC4;
            char n94[12];
            ((String *) (n94))->ctor_copy((String *) (nA0), false);
            void *u2 = gt->getText(itemId + 0x4fa);
            char n88[12];
            *(String *) n88 = *(String *) n94 + *(String *) u2;
            ((String *) ((*this->eventQueue)[idx]->name))->assign((String *) (n88));
            ((String *) (n88))->clear();
            ((String *) (n94))->clear();
            ((String *) (nA0))->clear();
            ((String *) (nC4))->clear();
            ((String *) (nAc))->clear();
            ((String *) (k34))->clear();
            return;
        }
        ((String *) (k34))->clear();
    }

    // fresh "+N <unit>" event
    this->cargoAggregateCount = count;
    char a0[12];
    ((String *) (a0))->ctor_int(this->cargoAggregateCount);
    char ac[12];
    ((String *) (ac))->ctor_char(g_Hud_ccUnit, false);
    char a94[12];
    *(String *) a94 = *(String *) a0 + *(String *) ac;
    char a88[12];
    ((String *) (a88))->ctor_copy((String *) (a94), false);
    void *unit = gt->getText(itemId + 0x4fa);
    char k34[12];
    *(String *) k34 = *(String *) a88 + *(String *) unit;
    ((String *) (&this->field_0x1f4))->assign((String *) (k34));
    ((String *) (k34))->clear();
    ((String *) (a88))->clear();
    ((String *) (a94))->clear();
    ((String *) (ac))->clear();
    ((String *) (a0))->clear();

    String *str = new String(this->field_0x1f4);
    ListItem *item = new ListItem(str);
    item->field_0x2c = itemId;
    if (!slotMode || extender) item->field_0x30 = 2;
    if (slotMode) item->field_0x24 = 1;
    addToEventQueue(item);
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_font; // *holder -> font String
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_canvas2; // *holder -> PaintCanvas
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_screenW; // *holder -> [0] = screen width

void Hud::drawEventString(String text, bool rightAlign) {
    void *font = *g_Hud_font;
    void *canvas = *g_Hud_canvas2;
    int x;
    if (this->letterbox == 0) {
        int base = this->eventLineMargin;
        int yBase = this->eventLineX;
        if (rightAlign == 0) {
            int w = gCanvas->GetTextWidth((unsigned) (long) (canvas), *(String *) (font));
            x = (base + 3) - w;
        } else {
            x = -3 - base;
        }
        x = x + yBase;
    } else {
        if (rightAlign == 0) {
            int margin = this->eventLineMarginAlt;
            int screenW = *(int *) *g_Hud_screenW;
            int w = gCanvas->GetTextWidth((unsigned) (long) (canvas), *(String *) (font));
            x = ((screenW - 1) - margin) - w;
        } else {
            x = this->eventLineMarginAlt + 1;
        }
    }
    char y = (char) (this->eventLineY - 1);
    gCanvas->DrawString((unsigned) (long) (font), text, (x), (y), false);
}

void Hud::setCurrentSecondaryWeapon(Item *item) {
    this->currentSecondaryWeapon = item;
    refreshQuickMenu(this);
}

int Hud::sameHudEventAsBeforeAggregate(String str) {
    Array<ListItem *> *q = this->eventQueue;
    int i = (int) q->size();
    ListItem *e;
    do {
        i = i + -1;
        if (i < 1)
            return -1;
        e = (*q)[i];
    } while (e == 0 || ((String *) e->name)->Compare_str(&str) != 0);
    return i;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern GameText **g_Hud_gameText;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_swCanvas; // font holder
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_swFont; // string holder
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_swScreenW; // [0]=screen width
extern const char g_Hud_swSep[] __attribute__((visibility("hidden"))); // " x" separator
extern const char g_Hud_swEnd[] __attribute__((visibility("hidden"))); // trailing string

void Hud::updateSecondaryWeaponString() {
    void *item = this->currentSecondaryWeapon;
    if (item == 0) return;

    GameText *gt = *g_Hud_gameText;
    int idx = ((Item *) (item))->getIndex();
    void *name = gt->getText(idx + 0x4fa);

    char sep[12], acc1[12], amount[12], acc2[12], end[12], acc3[12];
    ((String *) (sep))->ctor_char(g_Hud_swSep, false);
    *(String *) acc1 = *(String *) name + *(String *) sep;
    int amt = ((Item *) (item))->getAmount();
    ((String *) (amount))->ctor_int(amt);
    *(String *) acc2 = *(String *) acc1 + *(String *) amount;
    ((String *) (end))->ctor_char(g_Hud_swEnd, false);
    *(String *) acc3 = *(String *) acc2 + *(String *) end;

    ((String *) (&this->field_0x3b4))->assign((String *) (acc3));
    ((String *) (acc3))->clear();
    ((String *) (end))->clear();
    ((String *) (acc2))->clear();
    ((String *) (amount))->clear();
    ((String *) (acc1))->clear();
    ((String *) (sep))->clear();

    int screenW = *(int *) *g_Hud_swScreenW;
    int w = gCanvas->GetTextWidth((unsigned) (long) (*g_Hud_swCanvas), *(String *) (*g_Hud_swFont));
    this->secondaryLabelX = (screenW >> 1) - (w >> 1);
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_eqLetterbox; // *holder -> [0] byte
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_eqSelf; // *holder -> a Hud-like obj (+0x1e0/+0x1e4)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_eqScreenW; // *holder -> [0] width
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_eqFont; // *holder -> font String

void Hud::drawEventQueue() {
    char letterbox = *(char *) *g_Hud_eqLetterbox;
    char cinematicY = (letterbox == 0) ? 0 : (char) this->field_0x3e2;

    void *src = *g_Hud_eqSelf;
    // RAWREAD: src is an opaque "Hud-like" holder (void*) from a void** global; +0x1e0/+0x1e4
    // are int/float display state there, not Hud members (Hud has a String at +0x1e0).
    int dispBase = F<int>(src, 0x1e4);
    float dispScale = F<float>(src, 0x1e0);

    gCanvas->SetColor((unsigned char) (0xff), (unsigned char) (0xff), (unsigned char) (0xff), (unsigned char) (0));
    float mul = (letterbox == 0) ? -2.0f : -1.0f;
    int yOff = (int) (mul * dispScale);

    gCanvas->DrawImage2D((unsigned) this->eventBannerImage, this->field_0x3e0, 0);

    ListItem *item = (*this->eventQueue)[1];
    if (item != 0) {
        int kind = item->buttonKind;
        int b2, b3, b4;
        if (kind == 2) {
            b2 = 0;
            b3 = 0xed;
            b4 = 0;
        } else if (kind == 1) {
            b2 = 0xff;
            b3 = 0x2a;
            b4 = 0;
        } else if (kind == 3) {
            b2 = 0xff;
            b3 = 0x80;
            b4 = 0;
        } else {
            b2 = 0xff;
            b3 = 0xff;
            b4 = 0xff;
        }
        gCanvas->SetColor((unsigned char) (0xff), (unsigned char) (b2), (unsigned char) (b3), (unsigned char) (b4));

        String *label = (String *) item->name;
        void *font = *g_Hud_eqFont;
        int screenW = *(int *) *g_Hud_eqScreenW;
        int w = gCanvas->GetTextWidth((unsigned) (long) (gCanvas), *(String *) (font));
        char y = (char) ((char) yOff + (char) dispBase + cinematicY);
        gCanvas->DrawString((unsigned) (long) (font), *label, ((screenW >> 1) - w / 2), (y), false);
    }
    // restore the canvas colour to full white so the next HUD layer is not
    // tinted by the event-banner colour.
    gCanvas->SetColor(0xffffffffu);
}

unsigned int Hud::touchBegin(unsigned int a, unsigned int b, void *key) {
    unsigned int e = touchedElement(a, b);
    if (e == 0) {
        for (int i = 0; i != 0x19; i = i + 1) {
            if ((*this->keyArray)[i] == key) {
                this->touchFlags = this->touchFlags & ~(unsigned int) this->elementBits[i];
                this->elementBits[i] = 0;
                (*this->keyArray)[i] = 0;
            }
        }
    } else {
        unsigned int j;
        for (j = 0; j < 0x19; j = j + 1) {
            if ((*this->keyArray)[j] == key) {
                unsigned int v = (unsigned int) this->elementBits[j];
                if (e == v)
                    v = this->touchFlags;
                else
                    v = this->touchFlags & ~v;
                this->touchFlags = v | e;
                this->elementBits[j] = e;
                goto done;
            }
        }
        for (j = 0; j < 0x19; j = j + 1) {
            if ((*this->keyArray)[j] == 0) {
                (*this->keyArray)[j] = key;
                this->elementBits[j] = e;
                this->touchFlags = e | this->touchFlags;
                break;
            }
        }
    }
done:
    return this->touchFlags;
}

unsigned int Hud::sameHudEventAsBefore(String str) {
    Array<ListItem *> *q = this->eventQueue;
    int i = (int) q->size();
    while (--i >= 1) {
        ListItem *e = (*q)[i];
        if (e != 0 && ((String *) e->name)->Compare_str(&str) == 0)
            return 1;
    }
    return 0;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_initLayout; // *holder -> layout (+0x194,+0x198)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_initBound; // *holder -> [0] x bound
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_initOutX; // *holder -> int cell (x out)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_initOutY; // *holder -> int cell (y out)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern const unsigned short g_Hud_raceBadge[]; // race->atlas id
extern const char g_Hud_initMsg[] __attribute__((visibility("hidden")));

int Hud::init() {
    Hud_loadImages(this);

    this->messageActive = 0;
    this->hackingGameActive = 0;

    // key-state arrays: 0x19 slots each
    this->keyArray = new Array<void *>();
    this->keyArray->resize(0x19);
    this->elementBits = new int[0x19];
    for (int i = 0; i != 0x19; i++) {
        (*this->keyArray)[i] = 0;
        this->elementBits[i] = 0;
    }
    this->touchFlags = 0;

    // current system faction badge
    if (gStatus->inAlienOrbit() == 0) {
        int race = ((SolarSystem *) (((void *) (long) gStatus->getSystem())))->getRace();
        Image2DCreate(gCanvas, g_Hud_raceBadge[race], &this->factionLogoImage);
    }

    // seed the (empty) HUD message
    this->secondaryLabelTimerSeed = -1;
    this->secondaryLabelTimer = 0;
    {
        char tmp[12];
        ((String *) (tmp))->ctor_char(g_Hud_initMsg, false);
        ((String *) (&this->field_0x51c))->assign((String *) (tmp));
        ((String *) (tmp))->clear();
    }

    closeHudMenu();
    checkIfQuickMenuIsEmpty();
    releaseAllKeys();
    this->uintArray = 0;

    int *layout = (int *) *g_Hud_initLayout;
    int w = gCanvas->GetImage2DWidth((unsigned) (0));
    int bound = *(int *) *g_Hud_initBound;
    *(int *) *g_Hud_initOutX = (bound - w) - layout[0x65];
    *(int *) *g_Hud_initOutY = layout[0x66];
    return 0;
}

void Hud::drawPauseButton() {
    gCanvas->SetColor((unsigned) (-1));
    unsigned char flag = this->touchFlags;
    int y = this->field_0x40c;
    int x = this->field_0x40a;
    int img = (flag & 1) == 0 ? this->pauseButtonImage : this->pauseButtonPressedImage;
    return gCanvas->DrawImage2D((unsigned) (img), (x), (y));
}

// , then tail-calls the menu-refresh hook.

Hud *Hud::checkIfQuickMenuIsEmpty() {
    Ship *ship = gStatus->getShip();
    Array<Item *> *equip = ship->getEquipment(1);
    this->equipmentArray = equip;

    unsigned char empty;
    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip->size(); i++) {
            if ((*equip)[i] != 0) {
                hasSecondary = true;
                break;
            }
        }
    }
    if (hasSecondary) {
        empty = 0;
    } else if (ship->hasJumpDrive() == 0 && gStatus->getWingmen() == 0) {
        empty = (unsigned char) ship->hasCloak();
    } else {
        empty = 0;
    }
    this->quickMenuEmpty = empty;
    refreshQuickMenu(this);
    return this;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_dmLayout; // *holder -> layout obj
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_dmFont; // *holder -> font String
extern const char g_Hud_dmPrefix[] __attribute__((visibility("hidden")));

void Hud::drawMenu(int unused) {
    (void) unused;
    int *layout = (int *) *g_Hud_dmLayout;
    ((Layout *) (layout))->drawMask();

    // top cap
    gCanvas->DrawImage2D((unsigned) this->quickMenuTopImage, this->field_0x3c4 + this->menuOriginX, 0);
    // header glyph (centered)
    int hx = this->menuOriginX + this->field_0x3d4 + this->field_0x3dc / 2;
    char hy = (char) ((char) this->menuOriginYBase + (char) this->menuOriginY + (char) (this->menuRowHeight / 2)
                      - (char) layout[0x8b]);
    gCanvas->DrawImage2D((unsigned) this->quickMenuHeaderImage, hx, hy, (unsigned char) 0x11);

    int y = this->menuOriginY + this->menuOriginYBase + this->menuRowHeight;
    // repeated middle slices (one per button beyond the first)
    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        unsigned int count = (unsigned int) this->menuButtons->size();
        for (unsigned int i = 0; i < count - 1; i++) {
            gCanvas->DrawImage2D((unsigned) this->quickMenuMiddleImage, this->field_0x3c4 + this->menuOriginX, 0);
            y += this->field_0x3d0;
            count = (unsigned int) this->menuButtons->size();
        }
    }
    // bottom cap
    gCanvas->DrawImage2D((unsigned) this->quickMenuBottomImage, this->field_0x3c4 + this->menuOriginX, 0);

    // the actual buttons
    if (this->menuButtons != 0 && this->menuButtons->size() != 0) {
        unsigned int n = (unsigned int) this->menuButtons->size();
        for (unsigned int i = 0; i < n; i++) {
            (*this->menuButtons)[i]->draw();
            n = (unsigned int) this->menuButtons->size();
        }
    }

    if (this->menuLevel != 0) return;

    Ship *ship = gStatus->getShip();
    if (!ship->hasCloak() && ship->hasJumpDrive() == 0) return;

    char prefix[12], num[12], label[12];
    ((String *) (prefix))->ctor_char(g_Hud_dmPrefix, false);
    ((String *) (num))->ctor_int(this->fuelGaugeValue);
    *(String *) label = *(String *) prefix + *(String *) num;
    ((String *) (num))->clear();
    ((String *) (prefix))->clear();

    int gx = this->menuOriginX + this->field_0x3d4 + this->field_0x3dc / 2;
    unsigned char gy = (unsigned char) ((char) y + (char) (layout[0xc] / 2)
                                        + (char) layout[0xa2]);
    gCanvas->DrawImage2D((unsigned) this->fuelGaugeBarImage, gx, gy, (unsigned char) 0x11);
    gCanvas->DrawImage2D((unsigned) this->fuelGaugeIconImage, gx - layout[0x8c],
                         (char) layout[0xc] + (char) gy + (char) layout[0xa3], (unsigned char) 0x11);

    int barW = layout[0x8c];
    void *font = *g_Hud_dmFont;
    int ih = gCanvas->GetImage2DHeight((unsigned) (0));
    int th = gCanvas->GetTextHeight(0);
    char ty = (char) (((gy + (char) (ih / 2)) - (char) (th / 2)) + (char) layout[0x8d]);
    gCanvas->DrawString((unsigned) (long) (font), *(String *) (label), (barW + gx), (ty), false);
    ((String *) (label))->clear();
}

void Hud::clearQueue() {
    unsigned int i = 1;
    while (i < this->eventQueue->size()) {
        delete (*this->eventQueue)[i];
        (*this->eventQueue)[i] = 0;
        i = i + 1;
    }
    this->eventQueuePaused = 0;
}

void Hud::hudEvent(int eventId, PlayerEgo *ego, int arg) {
    switch (eventId) {
        case 1:
        case 2:
            // autofire on/off notice — only when the autofire UI is present
            if (this->hasAutofireUI == 0) return;
            hudEventBuild(this, eventId, ego, arg);
            return;
        case 3:
            if (this->hasBoostButton == 0 || ((PlayerEgo *) ((void *) (long) arg))->readyToBoost() == 0) return;
            hudEventBuild(this, eventId, ego, arg);
            return;
        case 4:
            if (this->hasBoostButton == 0) return;
            hudEventBuild(this, eventId, ego, arg);
            return;

        // ---- pure status-flag events (no queue entry) ----
        case 0x23:
            this->field_0x468 = 0;
            this->field_0x27a = 1;
            *(unsigned short *) &this->field_0x278 = 1;
            return;
        case 0x25:
            this->field_0x468 = 0;
            this->field_0x27a = 1;
            *(unsigned short *) &this->field_0x278 = 0x101;
            return;
        case 0x27:
            this->field_0x468 = 0;
            this->field_0x27a = 0;
            *(unsigned short *) &this->field_0x278 = 1;
            return;
        case 0x29:
            this->field_0x468 = 0;
            this->field_0x27a = 0;
            *(unsigned short *) &this->field_0x278 = 0x101;
            return;
        case 0x24:
        case 0x26:
        case 0x28:
        case 0x2a:
            // these clear the "showing" flag and set a fixed localized line, no queue
            this->field_0x278 = 0;
            hudEventBuild(this, eventId, ego, arg);
            return;

        default:
            // all remaining events compose a localized line and enqueue it
            hudEventBuild(this, eventId, ego, arg);
            return;
    }
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_csLayout; // *holder -> layout (+0x2c row pad)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_csStatus; // *holder -> status (+0x180 score,+0x184,+0x18c)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_csScreenW; // *holder -> [0] width
extern const char g_Hud_csZero[] __attribute__((visibility("hidden"))); // "0" pad

HUD_INLINE

void drawDigits(Hud *self, void *sprite, void *str, int x0, int y, int dw) {
    int len = (int) ((String *) str)->size();
    int x = x0;
    for (int i = 1; (unsigned int) (i - 1) < (unsigned int) len; i++) {
        char ch[12];
        ((String *) (ch))->SubString((String *) str, i - 1, i);
        int frame = ((String *) (ch))->ValueOf();
        ((String *) (ch))->clear();
        ((Sprite *) (sprite))->setFrame(frame);
        ((Sprite *) (sprite))->setPosition(x, y);
        ((Sprite *) (sprite))->draw(1.0f, 1.0f);
        x += dw;
    }
}

HUD_INLINE

void drawChallengeModeScoreImpl(Hud *self) {
    int *layout = (int *) *g_Hud_csLayout;
    int *status = (int *) *g_Hud_csStatus;
    int screenW = *(int *) *g_Hud_csScreenW;
    void *sprite = self->digitSprite;

    gCanvas->SetColor((unsigned) (-1));
    int fw = ((Sprite *) (sprite))->getFrameWidth();
    int pad = layout[0xb];
    int fh = ((Sprite *) (sprite))->getFrameHeight();
    int y = layout[0xb];

    // score string at status+0x184, right-padded to 7 digits
    char score[12];
    ((String *) (score))->ctor_int(status[0x61]);
    int slen = (int) ((String *) score)->size();
    if (slen < 7) {
        for (int k = 0; k < 7 - slen; k++) {
            char z[12], acc[12];
            ((String *) (z))->ctor_char(g_Hud_csZero, false);
            *(String *) acc = *(String *) z + *(String *) score;
            ((String *) (score))->assign((String *) (acc));
            ((String *) (acc))->clear();
            ((String *) (z))->clear();
        }
    }

    gCanvas->SetColor((unsigned) (-1));
    int dw = fw - pad;
    int half = screenW / 2;
    int span = (dw * 7) / 2;
    int startX = half - span;
    drawDigits(self, sprite, score, startX, y, dw);

    if (status[0x60] > 0 && status[0x63] > 1) {
        gCanvas->SetColor((unsigned) (-1));
        int yRow = y + fh + pad;
        int scoreVal = status[0x60];
        if (scoreVal < 0xbb9) {
            if (scoreVal % 100 >= 0x32) {
                int mult = status[0x63];
                float bonus = (float) mult;
                float base = (float) (mult * 1000);
                char bonusStr[12];
                ((String *) (bonusStr))->ctor_int((int) ((bonus * 0.0f + 1.0f) * base));
                int bl = (int) ((String *) bonusStr)->size();
                int bx = (screenW / 2 - ((bl * dw) >> 1));
                drawDigits(self, sprite, bonusStr, bx, fh + yRow + pad, dw);
                ((String *) (bonusStr))->clear();
            }
        }
        gCanvas->DrawImage2D((unsigned) self->multiplierIconImage, pad + startX, 0);

        char timeStr[12];
        ((String *) (timeStr))->ctor_int(status[0x63]);
        int tx = (half + pad) - span + gCanvas->GetImage2DWidth((unsigned) (0));
        drawDigits(self, sprite, timeStr, tx, yRow, dw);
        ((String *) (timeStr))->clear();
    }
    gCanvas->SetColor((unsigned) (-1));
    ((String *) (score))->clear();
}

void Hud::drawChallengeModeScore(int unused) {
    (void) unused;
    drawChallengeModeScoreImpl(this);
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern GameText **g_Hud_gameText;
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_meCanvas; // font holder
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_meFont; // string holder
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_meScreenW; // [0]=screen width
extern const char g_Hud_meSep[] __attribute__((visibility("hidden")));
extern const char g_Hud_meEnd[] __attribute__((visibility("hidden")));

void Hud::hudEventMedal(int medalId, int percent) {
    GameText *gt = *g_Hud_gameText;
    void *name = gt->getText(medalId + 0x5e3);

    char sep[12], acc1[12], num[12], acc2[12], end[12], acc3[12];
    ((String *) (sep))->ctor_char(g_Hud_meSep, false);
    *(String *) acc1 = *(String *) name + *(String *) sep;
    if (percent >= 100) percent = 100;
    ((String *) (num))->ctor_int(percent);
    *(String *) acc2 = *(String *) acc1 + *(String *) num;
    ((String *) (end))->ctor_char(g_Hud_meEnd, false);
    *(String *) acc3 = *(String *) acc2 + *(String *) end;

    void *dst = &this->field_0x1e0;
    ((String *) (dst))->assign((String *) (acc3));
    ((String *) (acc3))->clear();
    ((String *) (end))->clear();
    ((String *) (acc2))->clear();
    ((String *) (num))->clear();
    ((String *) (acc1))->clear();
    ((String *) (sep))->clear();

    char probe[12];
    ((String *) (probe))->ctor_copy((String *) (dst), false);
    int same = sameHudEventAsBefore(*(String *) probe);
    ((String *) (probe))->clear();
    if (same != 0) return;

    String *str = new String(*(String *) dst);
    ListItem *item = new ListItem(str, 3);
    addToEventQueue(item);

    int w = gCanvas->GetTextWidth((unsigned) (long) (*g_Hud_meCanvas), *(String *) (*g_Hud_meFont));
    int screenW = *(int *) *g_Hud_meScreenW;
    this->eventScrollTick = 0;
    this->eventScrolls = 1;
    this->letterbox = ((screenW / 2 - this->eventLineMargin) + this->eventLineMarginAlt * -2 < w) ? 1 : 0;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imLayout; // *holder -> layout obj
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imLetterbox; // *holder -> [0] byte
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imCargoA; // *holder -> obj (+0x54 cargo cur)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imCargoB; // *holder -> obj (+0x58 cargo max)
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imFlagA; // *holder -> [0] byte
__attribute__ ((visibility
(
"hidden"
)
)
)
extern void **g_Hud_imFlagB; // *holder -> [0] byte

void Hud::initHudMenu(int menuType, Level *lvl) {
    // tear down old menu buttons
    if (this->menuButtons != 0) {
        for (TouchButton *b: *this->menuButtons) delete b;
        this->menuButtons->clear();
        delete this->menuButtons;
        this->menuButtons = 0;
    }
    this->menuLevel = lvl;
    this->menuButtons = new Array<TouchButton *>();

    // refresh secondary-weapon equipment + label (equipment is owned by the Ship,
    // so the Array<Item*> wrapper itself is released but not its elements)
    delete this->equipmentArray;
    this->equipmentArray = gStatus->getShip()->getEquipment(1);
    updateSecondaryWeaponString();

    this->menuOriginX = 0;
    int *layout = (int *) *g_Hud_imLayout;
    int rowH = *(int *) (layout[0] + 0x1dc); // first row height
    // RAWREAD: layout is an opaque int* holder (from a void** global); layout[0] is an
    // untyped pointer with no modeled class, so +0x1dc has no named member.
    char letterbox = *(char *) *g_Hud_imLetterbox;

    int yOrigin;
    if (letterbox == 0) {
        yOrigin = this->menuBaseY;
    } else {
        // cargo-bay percentage shifts the menu up in letterboxed mode
        // RAWREAD: cargoA / *g_Hud_imCargoA / *g_Hud_imCargoB are opaque void* holders (from
        // void** globals); +0x54 (cargo cur) / +0x58 (cargo max) belong to an unmodeled class.
        void *cargoA = *g_Hud_imCargoA;
        float v;
        if ((long) this->menuLevel == 3)
            v = (float) *(int *) ((char *) cargoA + 0x54);
        else {
            v = (float) *(int *) ((char *) cargoA + 0x58);
            float adj = 0.0f;
            if (*(char *) *g_Hud_imFlagA == 0)
                adj = (*(char *) *g_Hud_imFlagB == 0) ? 1.0f : 0.0f;
            v = v - adj;
        }
        float yf = 0.0f;
        if (v >= 0.0f) {
            if ((long) this->menuLevel == 3)
                yf = (float) *(int *) ((char *) *g_Hud_imCargoA + 0x54);
            else {
                float v2 = (float) *(int *) ((char *) *g_Hud_imCargoB + 0x58);
                float adj = (*(char *) *g_Hud_imFlagA == 0)
                                ? ((*(char *) *g_Hud_imFlagB == 0) ? 1.0f : 0.0f)
                                : 0.0f;
                yf = v2 - adj;
            }
        }
        this->menuOriginY = (int) yf;
        yOrigin = ((this->menuRowHeight + (int) yf) - rowH / 2) + 1;
        this->menuBaseY = yOrigin;
    }
    (void) yOrigin;

    buildQuickMenu(this, menuType);
}

Hud::~Hud() {
    delete this->equipmentArray;
    this->equipmentArray = 0;

    delete this->eventQueue;
    this->eventQueue = 0;

    if (this->menuButtons != 0) {
        for (TouchButton *b: *this->menuButtons) delete b;
        this->menuButtons->clear();
        delete this->menuButtons;
    }
    this->menuButtons = 0;

    delete this->uintArray;
    this->uintArray = 0;
}

HUD_INLINE

void refreshQuickMenu(Hud *self) {
    self->updateSecondaryWeaponString();

    Ship *ship = gStatus->getShip();
    Array<Item *> *equip = ship->getEquipment(1);
    self->equipmentArray = equip;

    bool hasSecondary = false;
    if (equip != 0) {
        for (unsigned int i = 0; i < equip->size(); i++) {
            if ((*equip)[i] != 0) {
                hasSecondary = true;
                break;
            }
        }
    }
    unsigned char empty;
    if (hasSecondary) {
        empty = 0;
    } else if (ship->hasJumpDrive() == 0 && gStatus->getWingmen() == 0) {
        empty = (unsigned char) (ship->hasCloak() == 0);
    } else {
        empty = 0;
    }
    self->quickMenuEmpty = empty;
}

extern unsigned int g_Hud_heImportantMask; // 1<<id bitmask of priority events

HUD_INLINE

void hudEventBuild(Hud *self, int eventId, void *ego, int arg) {
    (void) ego;
    (void) arg;

    String *line = (String *) &self->field_0x1e0;
    char probe[12];
    ((String *) (probe))->ctor_copy(line, false);
    unsigned int dup = self->sameHudEventAsBefore(*(String *) probe);
    ((String *) (probe))->clear();
    if (dup != 0)
        return;

    String *str = new String(*line);
    // "important" ids get the alternate ListItem ctor that marks them priority.
    unsigned int idBit = (unsigned int) (eventId - 1);
    ListItem *item;
    if (idBit < 0x15 && ((1u << (idBit & 0x1f)) & g_Hud_heImportantMask) != 0)
        item = new ListItem(str, 1);
    else
        item = new ListItem(str, 0);
    self->addToEventQueue(item);

    void *font = *g_Hud_font;
    int w = gCanvas->GetTextWidth((unsigned) (long) (gCanvas), *(String *) (font));
    int screenW = *(int *) *g_Hud_screenW;
    self->eventScrollTick = 0;
    self->eventScrolls = 1;
    self->letterbox =
            (unsigned char) ((screenW / 2 - self->eventLineMargin) + self->eventLineMarginAlt * -2 < w);
}

static void buildQuickMenu(Hud *self, int menuType) {
    Hud_buildQuickMenu(self, menuType);
    self->quickMenuOpen = 1;
}

static void drawReticleAndBrackets(Hud *self, void *ego, unsigned int x, unsigned int y) {
    (void) x;
    (void) y;
    PaintCanvas *canvas = gCanvas;

    // Reticle image, tinted normally unless an interaction (autopilot/docking/
    // turret) suppresses the highlight; the lock bracket follows at +0x424/+0x41e.
    canvas->DrawImage2D((unsigned) self->reticleImage, self->field_0x42c, 0);

    unsigned char flags = self->touchFlags;
    unsigned short bx, by;
    int img;
    if ((flags & 0x40) != 0) {
        // (flags<<0x19)<0  -> locked bracket frozen
        bx = self->reticleX;
        by = self->reticleY;
        img = self->lockBracketLockedImage;
        self->lockBracketX = bx;
        self->lockBracketY = by;
    } else {
        bx = self->lockBracketX;
        by = self->lockBracketY;
        img = self->lockBracketImage;
    }
    canvas->DrawImage2D((unsigned) img, bx, by, '\x11');
    (void) ego;
}

__attribute__ ((visibility
(
"hidden"
)
)
)
extern Level **g_Hud_level; // *holder -> Level

static void drawRadar(Hud *self) {
    PaintCanvas *canvas = gCanvas;
    Status *st = gStatus;

    bool show = false;
    if (st->inAlienOrbit() == 0) {
        show = true;
    } else if (st->getCurrentCampaignMission() == 0x9a) {
        // alien-orbit mission 0x9a: only when there are docking targets
        Level *lvl = *g_Hud_level;
        if (lvl != 0 && lvl->getNumDockingTargets() > 0) show = true;
    }
    if (show && st->getCurrentCampaignMission() > 1) {
        int img = ((self->touchFlags & 0x80) != 0) // (flags<<0x19)<0
                      ? self->orbitMarkerActiveImage
                      : self->orbitMarkerIdleImage;
        canvas->DrawImage2D((unsigned) img, self->field_0x3f8, 0);
    }
}

static void drawBars(Hud *self, void *ego) {
    PaintCanvas *canvas = gCanvas;
    PlayerEgo *e = (PlayerEgo *) ego;
    Player *player = *(Player **) ego; // the ego embeds a Player* at +0
    float scale = (float) self->field_0x446;
    canvas->SetColor((unsigned) 0xffffffffu);

    // -- shield bar (only when the shield element +0x21f is enabled) --
    unsigned short barX = self->field_0x442;
    unsigned short barY = self->field_0x44a;
    if (self->hasShieldBar != 0) {
        int shp = player->getShieldHP();
        int frame = (shp < 2 || self->shieldHitFlash == 0) ? self->shieldFrameImage : self->shieldFrameHitImage;
        canvas->DrawImage2D((unsigned) frame, self->field_0x43c, self->field_0x442);
        canvas->DrawImage2D((unsigned) self->barDividerImage, self->field_0x43e, self->field_0x442);
        canvas->DrawImage2D((unsigned) self->shieldBarBgImage, self->field_0x440, self->field_0x44a);
        int rate = player->getShieldDamageRate();
        int w = (int) ((float) rate * scale);
        canvas->DrawRegion2D((unsigned) self->shieldBarFillImage, 0, 0, w, self->field_0x44c,
                             (float) w, 0, 0, 0, self->field_0x440);
        barX = self->field_0x444;
        barY = self->field_0x448;
    }

    // -- hull/armor bar --
    int ahp = player->getArmorHP();
    int aframe = (ahp < 1) ? self->armorFrameLowImage : self->armorFrameImage;
    canvas->DrawImage2D((unsigned) aframe, self->field_0x43c, barX);
    canvas->DrawImage2D((unsigned) self->barDividerImage, self->field_0x43e, barX);
    canvas->DrawImage2D((unsigned) self->armorBarBgImage, self->field_0x440, barY);
    int hrate = e->getHullDamageRate();
    int hw = (int) ((float) hrate * scale);
    canvas->DrawRegion2D((unsigned) self->armorBarFillImage, 0, 0, hw, self->field_0x44c,
                         (float) hw, 0, 0, 0, self->field_0x440);

    // -- armor regeneration overlay (element +0x220) --
    if (self->hasArmorRegen != 0) {
        int arate = player->getArmorDamageRate();
        int aw = (int) ((float) arate * scale);
        canvas->DrawRegion2D((unsigned) self->armorRegenFillImage, 0, 0, aw, self->field_0x44c,
                             (float) aw, 0, 0, 0, self->field_0x440);
    }
}

static void drawSecondaryWeaponPanel(Hud *self) {
    PaintCanvas *canvas = gCanvas;
    Level *lvl = *g_Hud_level;
    PlayerEgo *player = (PlayerEgo *) (lvl ? (void *) (long) lvl->getPlayer() : (void *) 0);

    if (player != 0 && player->hasAutoTurret() != 0) {
        bool on = player->autoTurretIsEnabled() != 0 || ((self->autoTurretFlags & 0x20) != 0);
        int img = on ? self->autoTurretOnImage : self->autoTurretOffImage;
        canvas->DrawImage2D((unsigned) img, self->field_0x3fe, 0);
    } else {
        // no auto-turret: replay the transient "weapon changed" label timer
        if (self->secondaryLabelTimer > 0) {
            void *font = *g_Hud_font;
            int screenW = *(int *) *g_Hud_screenW;
            unsigned short iconW = self->field_0x3ec;
            canvas->SetColor((unsigned char) 0xff, 0xff, 0xff, 0xff);
            canvas->DrawImage2D((unsigned) self->eventBannerImage, self->field_0x3ec, 0);
            int textW = gCanvas->GetTextWidth(
                (unsigned) (long) canvas, *(String *) (font));
            int tx = self->field_0x3ec + ((screenW - iconW) - textW) / 2;
            gCanvas->DrawString((unsigned) (long) canvas,
                                self->field_0x51c, tx, 0, false);
            canvas->SetColor((unsigned) 0xffffffffu);
            int t = self->secondaryLabelTimer;
            if (t > 4000) t = 0;
            self->secondaryLabelTimer = t;
        }
    }
}

static void drawMissionBanner(Hud *self) {
    PaintCanvas *canvas = gCanvas;
    canvas->SetColor((unsigned) 0xffffffffu);

    // banner background frame
    canvas->DrawImage2D((unsigned) self->missionBannerImage, self->field_0x438, 0);
}

static void drawMessage(Hud *self) {
    PaintCanvas *canvas = gCanvas;

    canvas->SetColor((unsigned char) 0xff, 0xff, 0xff, 0xff);
    void *font = *g_Hud_font;
    int screenW = *(int *) *g_Hud_screenW;
    int w = gCanvas->GetTextWidth((unsigned) (long) canvas, *(String *) (font));
    gCanvas->DrawString((unsigned) (long) canvas,
                        self->field_0x51c, screenW / 2 - w / 2, self->field_0x3e2, false);
    canvas->SetColor((unsigned) 0xffffffffu);
}

bool Hud::drawTitleImage(bool visible) {
    return visible;
}

