#include "Hud.h"


extern "C" void TouchButton_OnTouchEnd(int self, unsigned int a, void *b);
extern "C" void operator_delete(void *p);
extern "C" void Hud_drawReticleAndBrackets(Hud *self, void *ego, unsigned int x, unsigned int y);
extern "C" void Hud_drawRadar(Hud *self);
extern "C" void Hud_drawBars(Hud *self, void *ego);
extern "C" void Hud_drawSecondaryWeaponPanel(Hud *self);
extern "C" void Hud_drawOrbitInformation(Hud *self);
extern "C" void Hud_drawMissionBanner(Hud *self);
extern "C" void Hud_drawEventQueue(Hud *self);
extern "C" void Hud_drawMenu(Hud *self);
extern "C" void Hud_drawChallengeModeScore(Hud *self);
extern "C" void Hud_drawPauseButton(Hud *self);
extern "C" void Hud_drawMessage(Hud *self);
extern "C" int  Status_isChallengeMode();
extern "C" void Layout_drawBG();
extern "C" int  Status_inAlienOrbit();
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_SetColor4(void *canvas, int b1, int b2, int b3, int b4);
extern "C" int  PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, int anchor);
extern "C" void PaintCanvas_DrawString2(void *canvas, void *font, void *str, int x, char y);
extern "C" void *Status_getSystem();
extern "C" int  SolarSystem_hasNoOwner(void *sys);
extern "C" void *Status_getStation();
extern "C" void Station_getName(void *out, void *station);
extern "C" int  Status_getCurrentCampaignMission();
extern "C" int  SolarSystem_getSecurityLevel(void *sys);
extern "C" int  SolarSystem_getIndex(void *sys);
extern "C" void SolarSystem_getName(void *out, void *sys);
extern "C" void String_ctor_cstr(void *s, const char *cstr, bool b);
extern "C" void String_ctor_copy(void *s, void *src, bool b);
extern "C" void String_concat(void *out, void *lhs, void *rhs);
extern "C" void String_dtor(void *s);
extern "C" void *GameText_getText(void *gt, int id);
extern "C" unsigned int Hud_touchMoveFallback(Hud *self, unsigned int a, void *b);
extern "C" float Globals_sqrt(void *g, float v);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" int TouchButton_OnTouchBegin(void *btn, unsigned int x);
extern "C" void Hud_init(Hud *self);
extern "C" void String_op_assign(void *dst, void *src);
extern "C" void String_ctor_int(void *s, int v);
extern "C" void *operator_new(uint32_t);
extern "C" void ListItem_ctor1(void *li, void *str, int kind);
extern "C" void ListItem_ctor(void *li, void *str);
extern "C" int  Hud_sameHudEventAsBeforeAggregate(Hud *self, void *str);
extern "C" void Hud_addToEventQueue(Hud *self, void *item);
extern "C" void *Status_getMission();
extern "C" int  Mission_getType(void *m);
extern "C" void Status_replaceHash(void *out, void *tmpl, void *a, void *b, void *c);
extern "C" int  PaintCanvas_GetTextWidth(void *font, void *str);
extern "C" void Hud_secondaryWeaponChanged(Hud *self);
extern "C" int  Item_getIndex(void *item);
extern "C" int  Item_getAmount(void *item);
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, unsigned int frame);
extern "C" unsigned int Hud_touchedElement(Hud *self, unsigned int a, unsigned int b);
extern "C" void Image2DCreate(void *canvas, unsigned short id, void *outField);
extern "C" void *operator_new_arr(uint32_t);
extern "C" void Array_void_ctor(void *arr);
extern "C" void ArraySetLength_void(int n, void *arr);
extern "C" int  SolarSystem_getRace(void *sys);
extern "C" void Hud_closeHudMenu(Hud *self);
extern "C" void Hud_checkIfQuickMenuIsEmpty(Hud *self);
extern "C" void Hud_releaseAllKeys(Hud *self);
extern "C" void PaintCanvas_drawImage(void *canvas, int img, int x, int y);
extern "C" void *Status_getShip();
extern "C" void *Ship_getEquipment(void *ship, int slot);
extern "C" int  Ship_hasJumpDrive(void *ship);
extern "C" int  Status_getWingmen();
extern "C" unsigned char Ship_hasCloak(void *ship);
extern "C" void Layout_drawMask();
extern "C" void PaintCanvas_DrawImage2D2(void *canvas, int img, int xy);
extern "C" void PaintCanvas_DrawImage2D5(void *canvas, int img, int x, int y, char anchor);
extern "C" void TouchButton_draw(void *btn);
extern "C" int  Ship_hasCloak(void *ship);
extern "C" int  PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" int  PaintCanvas_GetTextHeight(void *canvas);
extern "C" int  PlayerEgo_readyToBoost(void *ego);
extern "C" int  Sprite_getFrameWidth(void *sprite);
extern "C" int  Sprite_getFrameHeight(void *sprite);
extern "C" void Sprite_setFrame(void *sprite, int frame);
extern "C" void Sprite_setPosition(void *sprite, int x, int y);
extern "C" void Sprite_draw(void *sprite);
extern "C" int  String_length(void *s);
extern "C" void String_SubString(void *out, void *s, int idx);
extern "C" int  String_ValueOf(void *s);
extern "C" int  Hud_sameHudEventAsBefore(Hud *self, void *str);
extern "C" void ListItem_ctor(void *li, void *str, int kind);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_Item_dtor(void *arr);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void Hud_updateSecondaryWeaponString(Hud *self);
extern "C" void *Array_Item_dtor(void *p);
extern "C" void *Array_ListItem_dtor(void *p);
extern "C" void *Array_uint_dtor(void *p);
extern "C" void String_dtor(void *p);

// ---- enableFireForTutorial_16651c.cpp ----
extern "C" void Hud_enableFireForTutorial(Hud *self, bool value)
{
    UC(self, 0x4a5) = value;
}

// ---- setTimeExtender_1624b8.cpp ----
extern "C" void Hud_setTimeExtender(Hud *self, bool p1, bool p2, bool p3, bool p4)
{
    UC(self, 0) = p1;
    UC(self, 0x280) = p3;
    UC(self, 0x281) = p4;
    if (p2 && p3) {
        I(self, 0x4c0) = 0x7d0;
        I(self, 0x4bc) = 0x50;
    }
}

// ---- playerHit_16323e.cpp ----
extern "C" void Hud_playerHit(Hud *self)
{
    UC(self, 0x244) = 1;
    I(self, 0x46c) = 0;
}

// ---- addToEventQueue_162488.cpp ----
extern "C" void Hud_addToEventQueue(Hud *self, ListItem *item)
{
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
extern "C" unsigned int Hud_firePressed(Hud *self)
{
    return (UC(self, 0x284) >> 4) & 1;
}

// ---- resetAnalogStick_166004.cpp ----
extern "C" void Hud_resetAnalogStick(Hud *self)
{
    I(self, 0x41e) = I(self, 0x424);
}

// ---- getAnalogY_16654a.cpp ----
extern "C" float Hud_getAnalogY(Hud *self)
{
    float num = (float)((int)US(self, 0x420) - (int)US(self, 0x426));
    float den = (float)I(self, 0x4e0);
    return num / den;
}

// ---- cargoFull_1636d8.cpp ----
extern "C" uint8_t Hud_cargoFull(Hud *self)
{
    return UC(self, 0x235);
}

// ---- touchEnd_166b84.cpp ----
extern "C" unsigned int Hud_touchEnd(Hud *self, unsigned int a, void *b, int key)
{
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
                TouchButton_OnTouchEnd(((int *)btns[1])[j], a, b);
                btns = (unsigned int *)P(self, 0x18);
            }
        }
    }
    return ret;
}

// ---- releaseAllKeys_161440.cpp ----
extern "C" void Hud_releaseAllKeys(Hud *self)
{
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

extern "C" void Hud_closeHudMenu(Hud *self)
{
    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) {
            void *p = Array_TouchButton_dtor(P(self, 0x18));
            operator_delete(p);
        }
        P(self, 0x18) = 0;
    }
    UC(self, 0x282) = 0;
}

// ---- getAnalogX_166522.cpp ----
extern "C" float Hud_getAnalogX(Hud *self)
{
    float num = (float)((int)US(self, 0x41e) - (int)US(self, 0x424));
    float den = (float)I(self, 0x4e0);
    return num / den;
}

// ---- setAutofireEnabled_161354.cpp ----
extern "C" void Hud_setAutofireEnabled(Hud *self, bool value)
{
    UC(self, 0x4a0) = value;
}

// ---- isHackingGameActive_1636e4.cpp ----
extern "C" uint8_t Hud_isHackingGameActive(Hud *self)
{
    return UC(self, 0x528);
}

// ---- setHackingGameActive_1636de.cpp ----
extern "C" void Hud_setHackingGameActive(Hud *self, bool value)
{
    UC(self, 0x528) = value;
}

// ---- setJumpMapSelected_166516.cpp ----
extern "C" void Hud_setJumpMapSelected(Hud *self, bool value)
{
    UC(self, 0x274) = value;
}

// ---- jumpMapSelected_166510.cpp ----
extern "C" uint8_t Hud_jumpMapSelected(Hud *self)
{
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


extern "C" void Hud_draw(Hud *self, long long t0, long long t1, void *ego,
                         bool letterbox, unsigned int x, unsigned int y)
{
    (void)t0; (void)t1;

    // background/letterbox state recorded for the panel renderers
    UC(self, 0x1ec) = (unsigned char)letterbox;

    // world-space HUD elements (reticle, target brackets, radar)
    Hud_drawReticleAndBrackets(self, ego, x, y);
    Hud_drawRadar(self);

    // player status (shield / armor / energy bars + secondary weapon)
    Hud_drawBars(self, ego);
    Hud_drawSecondaryWeaponPanel(self);

    // contextual banners
    Hud_drawOrbitInformation(self);
    Hud_drawMissionBanner(self);

    // transient event lines
    Hud_drawEventQueue(self);

    // the radial quick-menu, when open
    if (UC(self, 0x282) != 0)
        Hud_drawMenu(self);

    // challenge-mode score/time readout
    if (Status_isChallengeMode() != 0)
        Hud_drawChallengeModeScore(self);

    // pause button and any pending full-screen message
    Hud_drawPauseButton(self);
    if (UC(self, 0x4c8) != 0)
        Hud_drawMessage(self);
}

// ---- updateQueue_1624dc.cpp ----
extern "C" void *ListItem_dtor(void *p);   // ListItem::~ListItem

extern "C" void Hud_updateQueue(Hud *self, int dt)
{
    int t = I(self, 0x268) + dt;
    I(self, 0x268) = t;
    int v;
    if (t >= 0xfa1) {
        I(self, 0x268) = 0;
        int *p = (int *)I(self->f_264, 4);
        if ((void *)p[0] != 0) {
            void *q = ListItem_dtor((void *)p[0]);
            operator_delete(q);
            p = (int *)I(self->f_264, 4);
        }
        p[0] = 0;
        unsigned int i = 0;
        unsigned int d;
        while (true) {
            d = ((unsigned int *)self->f_264)[1];
            if (*(unsigned int *)self->f_264 <= i + 1)
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

extern "C" void Hud_drawOrbitInformation(Hud *self)
{
    if (Status_inAlienOrbit() != 0) return;

    void *canvas = *g_Hud_oiCanvas;
    int *layout = (int *)*g_Hud_oiLayout;
    PaintCanvas_SetColor(canvas, -1);
    int x = PaintCanvas_GetImage2DWidth(canvas) + layout[0x87]; // +0x21c

    Status_getSystem();
    if (SolarSystem_hasNoOwner(Status_getSystem()) == 0)
        PaintCanvas_DrawImage2D2(canvas, I(self, 0x1c4), 3);

    void *font = *g_Hud_oiFont;
    // station name
    {
        char name[12];
        Status_getStation();
        Station_getName(name, Status_getStation());
        PaintCanvas_DrawString2(canvas, font, name, x, (char)layout[0x88] /*+0x220*/);
        String_dtor(name);
    }
    PaintCanvas_SetColor(canvas, -1);

    if (Status_getCurrentCampaignMission() <= 0xf) return;

    void *sys = Status_getSystem();
    int sec = SolarSystem_getSecurityLevel(sys);
    Status_getSystem();
    int idx = SolarSystem_getIndex(Status_getSystem());
    int *status = (int *)*g_Hud_oiStatus;
    if (idx == 0x1a && status[0x45] /*+0x114*/ > 1) sec = 3;

    // system name + separator
    {
        char sysName[12], copy[12], sep[12], acc[12], full[12];
        Status_getSystem();
        SolarSystem_getName(sysName, Status_getSystem());
        String_ctor_copy(copy, sysName, false);
        String_ctor_cstr(sep, g_Hud_oiSep, false);
        String_concat(acc, copy, sep);
        void *txt = GameText_getText(*g_Hud_oiGameText, 0); // id resolved by table
        String_concat(full, acc, txt);
        PaintCanvas_DrawString2(canvas, font, full, x, (char)layout[0x89] /*+0x224*/);
        String_dtor(full);
        String_dtor(acc);
        String_dtor(sep);
        String_dtor(copy);
        String_dtor(sysName);
    }

    const unsigned char *row = g_Hud_secColors + sec * 0xc;
    PaintCanvas_SetColor4(canvas, row[0], row[4], row[8], 0xff);
    void *secTxt = GameText_getText(*g_Hud_oiGameText, sec);
    PaintCanvas_DrawString2(canvas, font, secTxt, x, (char)layout[0x8a] /*+0x228*/);
}

// ---- touchMove_166ab8.cpp ----
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

extern "C" unsigned int Hud_touchedElement(Hud *self, unsigned int x, unsigned int y)
{
    unsigned int *menu = (unsigned int *)self->f_18;
    if (UC(self, 0x282) != 0 && menu != 0) {
        // quick-menu open: delegate to its buttons
        for (unsigned int i = 0; i < menu[0]; i++) {
            if (TouchButton_OnTouchBegin(*(void **)(menu[1] + i * 4), x) != 0)
                return *(unsigned int *)*(void **)(I(self->f_18, 4) + i * 4);
            menu = (unsigned int *)self->f_18;
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
extern "C" void String_ctor(void *s);   // AbyssEngine::String::String()

extern "C" Hud *Hud_Hud(Hud *self)
{
    int i = 0x1c;
    do {
        String_ctor((char *)self + i);
        i = i + 0xc;
    } while (i != 0x10c);
    String_ctor((char *)self + 0x1e0);
    String_ctor((char *)self + 500);
    String_ctor((char *)self + 0x200);
    String_ctor((char *)self + 0x228);
    String_ctor((char *)self + 0x3b4);
    String_ctor((char *)self + 0x51c);
    Hud_init(self);
    return self;
}

// ---- catchCargo_16324c.cpp ----
// Hud::catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6,
//                 bool p7, int aggregateKey) — builds the cargo / docking / mission HUD event
// line and pushes it to the queue (aggregating repeated pickups of the same kind). r0=this.
extern "C" void Hud_catchCargoFinish(void *self); // DAT_001ac654 tail thunk (mission case)

__attribute__((visibility("hidden"))) extern void **g_Hud_ccGameText;
__attribute__((visibility("hidden"))) extern void **g_Hud_ccTemplate; // *holder -> base format String
extern const char g_Hud_ccHashX[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccHashN[]  __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit[]   __attribute__((visibility("hidden")));
extern const char g_Hud_ccUnit2[]  __attribute__((visibility("hidden")));

extern "C" void Hud_catchCargo(Hud *self, int amount, int cargoVal,
                               bool a, bool docked, bool mission, bool p6, bool p7,
                               int aggregateKey)
{
    I(self, 0x1d0) = 0;
    UC(self, 0x235) = (unsigned char)docked;

    if (mission) {
        // mission cargo: two replaceHash passes producing the localized line
        void *gt = *g_Hud_ccGameText;
        void *base = GameText_getText(gt, 0x219);
        void *dst = B(self, 0x1f4);
        String_op_assign(dst, base);

        void *tmpl = *g_Hud_ccTemplate;
        char a40[12]; String_ctor_copy(a40, dst, false);
        Status_getMission();
        Mission_getType(Status_getMission());
        void *typeTxt = GameText_getText(gt, 0);
        char a4c[12]; String_ctor_copy(a4c, typeTxt, false);
        char a58[12]; String_ctor_cstr(a58, g_Hud_ccHashX, false);
        char out1[12]; Status_replaceHash(out1, tmpl, a40, a4c, a58);
        String_op_assign(dst, out1);
        String_dtor(out1); String_dtor(a58); String_dtor(a4c); String_dtor(a40);

        tmpl = *g_Hud_ccTemplate;
        char a64[12]; String_ctor_copy(a64, dst, false);
        char a70[12]; String_ctor_int(a70, 1);
        char a7c[12]; String_ctor_cstr(a7c, g_Hud_ccHashN, false);
        char out2[12]; Status_replaceHash(out2, tmpl, a64, a70, a7c);
        String_op_assign(dst, out2);
        String_dtor(out2); String_dtor(a7c); String_dtor(a70); String_dtor(a64);

        void *item = operator_new(0x48);
        void *str = operator_new(0xc);
        String_ctor_copy(str, dst, false);
        ListItem_ctor(item, str);
        *(int *)((char *)item + 0x2c) = cargoVal;
        Hud_addToEventQueue(self, item);
        return;
    }

    if (docked) {
        // docking confirmation line
        void *gt = *g_Hud_ccGameText;
        void *txt = GameText_getText(gt, 0x18a);
        String_op_assign(B(self, 0x1f4), txt);
        void *item = operator_new(0x48);
        void *str = operator_new(0xc);
        String_ctor_copy(str, B(self, 0x1f4), false);
        ListItem_ctor1(item, str, 1);
        Hud_catchCargoFinish(self);
        return;
    }

    if (!a) return; // amount==0 fast path (LAB_001734a6)

    void *gt = *g_Hud_ccGameText;

    // aggregate with previous "+N <unit>" event if allowed
    if (aggregateKey != 0 && self->f_26c != 0) {
        char a0[12]; String_ctor_int(a0, I(self, 0x52c));
        char ac[12]; String_ctor_cstr(ac, g_Hud_ccUnit, false);
        char a94[12]; String_concat(a94, a0, ac);
        char a88[12]; String_ctor_copy(a88, a94, false);
        void *unit = GameText_getText(gt, 0);
        char k34[12]; String_concat(k34, a88, unit);
        String_dtor(a88); String_dtor(a94); String_dtor(ac); String_dtor(a0);

        char b8[12]; String_ctor_copy(b8, k34, false);
        int idx = Hud_sameHudEventAsBeforeAggregate(self, b8);
        String_dtor(b8);
        if (idx >= 0) {
            I(self, 0x268) = 2000;
            int newAmt = I(self, 0x52c) + (a ? 1 : 0);
            I(self, 0x52c) = newAmt;
            char nAc[12]; String_ctor_int(nAc, newAmt);
            char nC4[12]; String_ctor_cstr(nC4, g_Hud_ccUnit2, false);
            char nA0[12]; String_concat(nA0, nAc, nC4);
            char n94[12]; String_ctor_copy(n94, nA0, false);
            void *u2 = GameText_getText(gt, 0);
            char n88[12]; String_concat(n88, n94, u2);
            String_op_assign(*(void **)(*(int *)(I(self->f_264, 4) + idx * 4) + 0x1c), n88);
            String_dtor(n88); String_dtor(n94); String_dtor(nA0); String_dtor(nC4); String_dtor(nAc);
            String_dtor(k34);
            return;
        }
        String_dtor(k34);
    }

    // fresh "+N <unit>" event
    I(self, 0x52c) = (a ? 1 : 0);
    char a0[12]; String_ctor_int(a0, I(self, 0x52c));
    char ac[12]; String_ctor_cstr(ac, g_Hud_ccUnit, false);
    char a94[12]; String_concat(a94, a0, ac);
    char a88[12]; String_ctor_copy(a88, a94, false);
    void *unit = GameText_getText(gt, 0);
    char k34[12]; String_concat(k34, a88, unit);
    String_op_assign(B(self, 0x1f4), k34);
    String_dtor(k34); String_dtor(a88); String_dtor(a94); String_dtor(ac); String_dtor(a0);

    void *item = operator_new(0x48);
    void *str = operator_new(0xc);
    String_ctor_copy(str, B(self, 0x1f4), false);
    ListItem_ctor(item, str);
    *(int *)((char *)item + 0x2c) = cargoVal;
    if (!p7 || p6) *(int *)((char *)item + 0x30) = 2;
    if (p7) *(unsigned char *)((char *)item + 0x24) = 1;
    Hud_addToEventQueue(self, item);
}

// ---- drawEventString_1636f4.cpp ----
// Hud::drawEventString(String text, bool rightAlign) — draws a HUD event line, right- or
// left-justified depending on rightAlign and the cinematic-letterbox flag at +0x1ec, then
// emits it via PaintCanvas::DrawString. r0=this, r1=text, r2=rightAlign.

__attribute__((visibility("hidden"))) extern void **g_Hud_font;   // *holder -> font String
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas2;// *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_screenW;// *holder -> [0] = screen width

extern "C" void Hud_drawEventString(Hud *self, void *text, int rightAlign)
{
    void *font = *g_Hud_font;
    void *canvas = *g_Hud_canvas2;
    int x;
    if (UC(self, 0x1ec) == 0) {
        int base = I(self, 0x4e8);
        int yBase = I(self, 0x160);
        if (rightAlign == 0) {
            int w = PaintCanvas_GetTextWidth(canvas, font);
            x = (base + 3) - w;
        } else {
            x = -3 - base;
        }
        x = x + yBase;
    } else {
        if (rightAlign == 0) {
            int margin = I(self, 0x4f0);
            int screenW = *(int *)*g_Hud_screenW;
            int w = PaintCanvas_GetTextWidth(canvas, font);
            x = ((screenW - 1) - margin) - w;
        } else {
            x = I(self, 0x4f0) + 1;
        }
    }
    char y = (char)(I(self, 0x164) - 1);
    PaintCanvas_DrawString2(canvas, font, text, x, y);
}

// ---- setCurrentSecondaryWeapon_1622ec.cpp ----
extern "C" void Hud_setCurrentSecondaryWeapon(Hud *self, Item *item)
{
    self->f_258 = item;
    return Hud_secondaryWeaponChanged(self);
}

// ---- sameHudEventAsBeforeAggregate_163200.cpp ----
extern "C" int String_Compare(String *a, String *b);   // AbyssEngine::String::Compare

extern "C" int Hud_sameHudEventAsBeforeAggregate(Hud *self, String *str)
{
    int i = *(int *)self->f_264;
    int e;
    do {
        i = i + -1;
        if (i < 1)
            return -1;
        e = I((void *)(I(self->f_264, 4)), i * 4);
    } while (e == 0 || String_Compare((String *)I((void *)e, 0x1c), str) != 0);
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

extern "C" void Hud_updateSecondaryWeaponString(Hud *self)
{
    void *item = self->f_258;
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

// ---- drawEventQueue_1622f4.cpp ----
// Hud::drawEventQueue() — draws the sliding event-banner background image and, if the queue
// front holds a ListItem, its colored label (color keyed by the item's kind at +0x30). The
// vertical offset animates from the queue timer at +0x268 / display +0x1e0. r0=this.
extern "C" void Hud_eventQueueFinish(void *canvas, unsigned int color); // DAT_001ac094 tail thunk

__attribute__((visibility("hidden"))) extern void **g_Hud_eqLetterbox; // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_eqSelf;      // *holder -> a Hud-like obj (+0x1e0/+0x1e4)
__attribute__((visibility("hidden"))) extern void **g_Hud_eqCanvas;    // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_eqScreenW;   // *holder -> [0] width
__attribute__((visibility("hidden"))) extern void **g_Hud_eqFont;      // *holder -> font String

extern "C" void Hud_drawEventQueue(Hud *self)
{
    char letterbox = *(char *)*g_Hud_eqLetterbox;
    char cinematicY = (letterbox == 0) ? 0 : (char)US(self, 0x3e2);

    void *src = *g_Hud_eqSelf;
    void *canvas = *g_Hud_eqCanvas;
    int dispBase = I(src, 0x1e4);
    float dispScale = F(src, 0x1e0);

    PaintCanvas_SetColor4(canvas, 0xff, 0xff, 0xff, 0); // alpha derived below replaced inline
    float mul = (letterbox == 0) ? -2.0f : -1.0f;
    int yOff = (int)(mul * dispScale);

    PaintCanvas_DrawImage2D2(canvas, I(self, 0x354), US(self, 0x3e0));

    int item = *(int *)(I(self->f_264, 4) + 4);
    if (item != 0) {
        int kind = *(int *)(item + 0x30);
        int b2, b3, b4;
        if (kind == 2)      { b2 = 0;    b3 = 0xed; b4 = 0; }
        else if (kind == 1) { b2 = 0xff; b3 = 0x2a; b4 = 0; }
        else if (kind == 3) { b2 = 0xff; b3 = 0x80; b4 = 0; }
        else                { b2 = 0xff; b3 = 0xff; b4 = 0xff; }
        PaintCanvas_SetColor4(canvas, 0xff, b2, b3, b4);

        int strVal = *(int *)(item + 0x1c);
        void *font = *g_Hud_eqFont;
        int screenW = *(int *)*g_Hud_eqScreenW;
        int w = PaintCanvas_GetTextWidth(canvas, font);
        char y = (char)((char)yOff + (char)dispBase + cinematicY);
        PaintCanvas_DrawString2(canvas, font, (void *)(long)strVal, (screenW >> 1) - w / 2, y);
    }
    Hud_eventQueueFinish(canvas, 0xffffffff);
}

// ---- touchBegin_1669fc.cpp ----
extern "C" unsigned int Hud_touchBegin(Hud *self, unsigned int a, void *b, int key)
{
    unsigned int e = Hud_touchedElement(self, a, (unsigned int)b);
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
extern "C" int String_Compare(String *a, String *b);   // AbyssEngine::String::Compare

extern "C" unsigned int Hud_sameHudEventAsBefore(Hud *self, String *str)
{
    int i = *(int *)self->f_264;
    while (--i >= 1) {
        int e = ((int *)I(self->f_264, 4))[i];
        if (e != 0 && String_Compare((String *)I((void *)e, 0x1c), str) == 0)
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
extern "C" void Hud_loadImages(Hud *self);   // the 70+ Image2DCreate calls

__attribute__((visibility("hidden"))) extern void **g_Hud_initCanvas;  // *holder -> PaintCanvas
__attribute__((visibility("hidden"))) extern void **g_Hud_initLayout;  // *holder -> layout (+0x194,+0x198)
__attribute__((visibility("hidden"))) extern void **g_Hud_initBound;   // *holder -> [0] x bound
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutX;    // *holder -> int cell (x out)
__attribute__((visibility("hidden"))) extern void **g_Hud_initOutY;    // *holder -> int cell (y out)
__attribute__((visibility("hidden"))) extern const unsigned short g_Hud_raceBadge[]; // race->atlas id
extern const char g_Hud_initMsg[] __attribute__((visibility("hidden")));

extern "C" int Hud_init(Hud *self)
{
    Hud_loadImages(self);

    UC(self, 0x4c8) = 0;
    UC(self, 0x528) = 0;

    // key-state arrays: 0x19 slots each
    void *keys = operator_new(0xc);
    Array_void_ctor(keys);
    self->f_28c = keys;
    ArraySetLength_void(0x19, keys);
    void *bits = operator_new_arr(100);
    self->f_290 = bits;
    for (int i = 0; i != 0x19; i++) {
        *(int *)(I(self->f_28c, 4) + i * 4) = 0;
        *(int *)(I(self, 0x290) + i * 4) = 0;
    }
    I(self, 0x284) = 0;

    // current system faction badge
    if (Status_inAlienOrbit() == 0) {
        void *canvas = *g_Hud_initCanvas;
        Status_getSystem();
        int race = SolarSystem_getRace(Status_getSystem());
        Image2DCreate(canvas, g_Hud_raceBadge[race], B(self, 0x1c4));
    }

    // seed the (empty) HUD message
    I(self, 0x514) = -1;
    I(self, 0x518) = 0;
    {
        char tmp[12];
        String_ctor_cstr(tmp, g_Hud_initMsg, false);
        String_op_assign(B(self, 0x51c), tmp);
        String_dtor(tmp);
    }

    Hud_closeHudMenu(self);
    Hud_checkIfQuickMenuIsEmpty(self);
    Hud_releaseAllKeys(self);
    self->f_530 = 0;

    int *layout = (int *)*g_Hud_initLayout;
    int w = PaintCanvas_GetImage2DWidth(*g_Hud_initCanvas);
    int bound = *(int *)*g_Hud_initBound;
    *(int *)*g_Hud_initOutX = (bound - w) - layout[0x65]; // +0x194
    *(int *)*g_Hud_initOutY = layout[0x66];               // +0x198
    return 0;
}

// ---- drawPauseButton_1637a4.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_Hud_canvas;

extern "C" void Hud_drawPauseButton(Hud *self)
{
    void **holder = g_Hud_canvas;
    PaintCanvas_SetColor(*holder, -1);
    unsigned char flag = UC(self, 0x284);
    int y = US(self, 0x40c);
    int x = US(self, 0x40a);
    int img = (flag & 1) == 0 ? I(self, 0x2f8) : I(self, 0x2f4);
    return PaintCanvas_drawImage(*holder, img, x, y);
}

// ---- checkIfQuickMenuIsEmpty_1613d0.cpp ----
// Hud::checkIfQuickMenuIsEmpty() — decides whether the radial quick-menu has any usable
// entry (a secondary weapon, jump drive, wingmen, or cloak) and stores the "empty" flag at
// +0x283, then tail-calls the menu-refresh hook.
extern "C" unsigned char Ship_hasCloakNeg(void *ship); // the final inverted-cloak accessor
extern "C" void Hud_refreshQuickMenu(Hud *self);       // DAT_001ac644 tail thunk

extern "C" Hud *Hud_checkIfQuickMenuIsEmpty(Hud *self)
{
    void *ship = Status_getShip();
    unsigned int *equip = (unsigned int *)Ship_getEquipment(ship, 1);
    self->f_25c = equip;

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
        Status_getShip();
        if (Ship_hasJumpDrive(Status_getShip()) == 0 && Status_getWingmen() == 0) {
            Status_getShip();
            empty = (unsigned char)(Ship_hasCloakNeg(Status_getShip()) ^ 1);
        } else {
            empty = 0;
        }
    }
    UC(self, 0x283) = empty;
    Hud_refreshQuickMenu(self);
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

extern "C" void Hud_drawMenu(Hud *self)
{
    Layout_drawMask();
    void *canvas = *g_Hud_dmCanvas;
    int *layout = (int *)*g_Hud_dmLayout;

    // top cap
    PaintCanvas_DrawImage2D2(canvas, I(self, 0x298), I(self, 0x3c4) + I(self, 0x4cc));
    // header glyph (centered)
    int hx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    char hy = (char)((char)I(self, 0x4d0) + (char)I(self, 0x3c8) + (char)(I(self, 0x3cc) / 2)
                     - (char)layout[0x8b] /*+0x22c*/);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x35c), hx, hy, 0x11);

    int y = I(self, 0x3c8) + I(self, 0x4d0) + I(self, 0x3cc);
    // repeated middle slices (one per button beyond the first)
    if (self->f_18 != 0 && *(int *)self->f_18 != 0) {
        int count = *(int *)self->f_18;
        for (unsigned int i = 0; i < (unsigned int)(count - 1); i++) {
            PaintCanvas_DrawImage2D2(canvas, I(self, 0x2a0), I(self, 0x3c4) + I(self, 0x4cc));
            y += I(self, 0x3d0);
            count = *(int *)self->f_18;
        }
    }
    // bottom cap
    PaintCanvas_DrawImage2D2(canvas, I(self, 0x29c), I(self, 0x3c4) + I(self, 0x4cc));

    // the actual buttons
    if (self->f_18 != 0 && *(unsigned int *)self->f_18 != 0) {
        unsigned int n = *(unsigned int *)self->f_18;
        for (unsigned int i = 0; i < n; i++) {
            TouchButton_draw(((void **)I(self->f_18, 4))[i]);
            n = *(unsigned int *)self->f_18;
        }
    }

    if (I(self, 0x238) != 0) return;

    Status_getShip();
    int cloak = Ship_hasCloak(Status_getShip());
    if (cloak == 0) {
        Status_getShip();
        if (Ship_hasJumpDrive(Status_getShip()) == 0) return;
    }

    char prefix[12], num[12], label[12];
    String_ctor_cstr(prefix, g_Hud_dmPrefix, false);
    String_ctor_int(num, I(self, 0x27c));
    String_concat(label, prefix, num);
    String_dtor(num);
    String_dtor(prefix);

    int gx = I(self, 0x4cc) + I(self, 0x3d4) + I(self, 0x3dc) / 2;
    unsigned char gy = (unsigned char)((char)y + (char)(layout[0xc] /*+0x30*/ / 2)
                        + (char)layout[0xa2] /*+0x288*/);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x374), gx, gy, 0x11);
    PaintCanvas_DrawImage2D5(canvas, I(self, 0x370), gx - layout[0x8c] /*+0x230*/,
        (char)layout[0xc] + (char)gy + (char)layout[0xa3] /*+0x28c*/, 0x11);

    int barW = layout[0x8c];
    void *font = *g_Hud_dmFont;
    int ih = PaintCanvas_GetImage2DHeight(canvas);
    int th = PaintCanvas_GetTextHeight(canvas);
    char ty = (char)(((gy + (char)(ih / 2)) - (char)(th / 2)) + (char)layout[0x8d] /*+0x234*/);
    PaintCanvas_DrawString2(canvas, font, label, barW + gx, ty);
    String_dtor(label);
}

// ---- clearQueue_162550.cpp ----
extern "C" void *ListItem_dtor(void *p);   // ListItem::~ListItem

extern "C" void Hud_clearQueue(Hud *self)
{
    unsigned int i = 1;
    int off = 4;
    unsigned int *arr;
    while (arr = (unsigned int *)self->f_264, i < arr[0]) {
        unsigned int data = arr[1];
        void *item = *(void **)(data + off);
        int *dst;
        if (item == 0) {
            dst = (int *)(data + i * 4);
        } else {
            void *p = ListItem_dtor(item);
            operator_delete(p);
            dst = (int *)(I(self->f_264, 4) + off);
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
extern "C" void Hud_hudEventBuild(Hud *self, int eventId, void *ego, int arg); // builds + queues

extern "C" void Hud_hudEvent(Hud *self, int eventId, void *ego, int arg)
{
    switch (eventId) {
    case 1:
    case 2:
        // autofire on/off notice — only when the autofire UI is present
        if (self->f_221 == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;
    case 3:
        if (self->f_21e == 0 || PlayerEgo_readyToBoost((void *)(long)arg) == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;
    case 4:
        if (self->f_21e == 0) return;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;

    // ---- pure status-flag events (no queue entry) ----
    case 0x23:
        I(self, 0x468) = 0;
        self->f_27a = 1;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x25:
        I(self, 0x468) = 0;
        self->f_27a = 1;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x27:
        I(self, 0x468) = 0;
        self->f_27a = 0;
        *(unsigned short *)B(self, 0x278) = 1;
        return;
    case 0x29:
        I(self, 0x468) = 0;
        self->f_27a = 0;
        *(unsigned short *)B(self, 0x278) = 0x101;
        return;
    case 0x24:
    case 0x26:
    case 0x28:
    case 0x2a:
        // these clear the "showing" flag and set a fixed localized line, no queue
        self->f_278 = 0;
        Hud_hudEventBuild(self, eventId, ego, arg);
        return;

    default:
        // all remaining events compose a localized line and enqueue it
        Hud_hudEventBuild(self, eventId, ego, arg);
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
        String_SubString(ch, str, i - 1);
        int frame = String_ValueOf(ch);
        String_dtor(ch);
        Sprite_setFrame(sprite, frame);
        Sprite_setPosition(sprite, x, y);
        Sprite_draw(sprite);
        x += dw;
    }
}

extern "C" void Hud_drawChallengeModeScore(Hud *self)
{
    void *canvas = *g_Hud_csCanvas;
    int *layout = (int *)*g_Hud_csLayout;
    int *status = (int *)*g_Hud_csStatus;
    int screenW = *(int *)*g_Hud_csScreenW;
    void *sprite = self->f_534;

    PaintCanvas_SetColor(canvas, -1);
    int fw = Sprite_getFrameWidth(sprite);
    int pad = layout[0xb]; // +0x2c
    int fh = Sprite_getFrameHeight(sprite);
    int y = layout[0xb];

    // score string at status+0x184, right-padded to 7 digits
    char score[12];
    String_ctor_int(score, status[0x61] /*+0x184*/);
    int slen = String_length(score);
    if (slen < 7) {
        for (int k = 0; k < 7 - slen; k++) {
            char z[12], acc[12];
            String_ctor_cstr(z, g_Hud_csZero, false);
            String_concat(acc, z, score);
            String_op_assign(score, acc);
            String_dtor(acc);
            String_dtor(z);
        }
    }

    PaintCanvas_SetColor(canvas, -1);
    int dw = fw - pad;
    int half = screenW / 2;
    int span = (dw * 7) / 2;
    int startX = half - span;
    drawDigits(self, sprite, score, startX, y, dw);

    if (status[0x60] /*+0x180*/ > 0 && status[0x63] /*+0x18c*/ > 1) {
        PaintCanvas_SetColor(canvas, -1);
        int yRow = y + fh + pad;
        int scoreVal = status[0x60];
        if (scoreVal < 0xbb9) {
            if (scoreVal % 100 >= 0x32) {
                int mult = status[0x63];
                float bonus = (float)mult;
                float base = (float)(mult * 1000);
                char bonusStr[12];
                String_ctor_int(bonusStr, (int)((bonus * 0.0f + 1.0f) * base));
                int bl = String_length(bonusStr);
                int bx = (screenW / 2 - ((bl * dw) >> 1));
                drawDigits(self, sprite, bonusStr, bx, fh + yRow + pad, dw);
                String_dtor(bonusStr);
            }
        }
        PaintCanvas_DrawImage2D2(canvas, I(self, 0x538), pad + startX);

        char timeStr[12];
        String_ctor_int(timeStr, status[0x63]);
        int tx = (half + pad) - span + PaintCanvas_GetImage2DWidth(canvas);
        drawDigits(self, sprite, timeStr, tx, yRow, dw);
        String_dtor(timeStr);
    }
    PaintCanvas_SetColor(canvas, -1);
    String_dtor(score);
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

// ---- initHudMenu_1615c8.cpp ----
// Hud::initHudMenu(int menuType, Level* lvl) — rebuilds the radial quick-menu. Tears down and
// recreates the menu button array (+0x18), refreshes the secondary-weapon equipment list and
// label, recomputes the menu's vertical origin from the cargo-bay fill (letterboxed layouts
// shift it up), then assembles the type-specific buttons. r0=this, r1=menuType, r2=lvl.
//
// The per-type button assembly (weapon list, jump targets, autopilot routes, etc.) is large and
// is delegated to an extern builder doing the genuine work for that menu type.
extern "C" void Hud_buildQuickMenu(Hud *self, int menuType); // per-type button assembly

__attribute__((visibility("hidden"))) extern void **g_Hud_imLayout;     // *holder -> layout (*piVar6, +0x1dc,+0x30)
__attribute__((visibility("hidden"))) extern void **g_Hud_imLetterbox;  // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoA;     // *holder -> obj (+0x54 cargo cur)
__attribute__((visibility("hidden"))) extern void **g_Hud_imCargoB;     // *holder -> obj (+0x58 cargo max)
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagA;      // *holder -> [0] byte
__attribute__((visibility("hidden"))) extern void **g_Hud_imFlagB;      // *holder -> [0] byte

extern "C" void Hud_initHudMenu(Hud *self, int menuType, void *lvl)
{
    // tear down old menu buttons
    if (self->f_18 != 0) {
        ArrayReleaseClasses_TouchButton(self->f_18);
        if (self->f_18 != 0) operator_delete(Array_TouchButton_dtor(self->f_18));
        self->f_18 = 0;
    }
    void *arr = operator_new(0xc);
    Array_TouchButton_ctor(arr);
    self->f_238 = lvl;
    self->f_18 = arr;

    // refresh secondary-weapon equipment + label
    if (self->f_25c != 0) operator_delete(Array_Item_dtor(self->f_25c));
    self->f_25c = 0;
    void *ship = Status_getShip();
    self->f_25c = Ship_getEquipment(ship, 1);
    Hud_updateSecondaryWeaponString(self);

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

    Hud_buildQuickMenu(self, menuType);
}

// ---- _Hud_16126c.cpp ----
// Hud::~Hud() — releases the equipment array, the event-queue array, the touch-button array,
// the uint array at +0x530, six engine objects (via a shared dtor thunk), and the block of
// String members from +0x10 up to +0x100. Returns this.
extern "C" void Hud_subObjectDtor(void *p);   // DAT_0017134c shared sub-object dtor thunk

extern "C" Hud *Hud_dtor(Hud *self)
{
    if (self->f_25c != 0) operator_delete(Array_Item_dtor(self->f_25c));
    self->f_25c = 0;

    if (self->f_264 != 0) operator_delete(Array_ListItem_dtor(self->f_264));
    self->f_264 = 0;

    if (self->f_18 != 0) {
        ArrayReleaseClasses_TouchButton(self->f_18);
        if (self->f_18 != 0) operator_delete(Array_TouchButton_dtor(self->f_18));
    }
    self->f_18 = 0;

    if (self->f_530 != 0) operator_delete(Array_uint_dtor(self->f_530));
    self->f_530 = 0;

    Hud_subObjectDtor(B(self, 0x51c));
    Hud_subObjectDtor(B(self, 0x3b4));
    Hud_subObjectDtor(B(self, 0x228));
    Hud_subObjectDtor(B(self, 0x200));
    Hud_subObjectDtor(B(self, 0x1f4));
    Hud_subObjectDtor(B(self, 0x1e0));

    for (int off = 0x100; off != 0x10; off -= 0xc)
        String_dtor(B(self, off));

    return self;
}
