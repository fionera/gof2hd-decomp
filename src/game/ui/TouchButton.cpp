#include "gof2/TouchButton.h"
#include "gof2/FModSound.h"
#include "gof2/Achievements.h"
#include "gof2/Layout.h"
#include "gof2/String.h"
#include "gof2/PaintCanvas.h"

extern void *g_PaintCanvas;

extern "C" void TB_assignString(void *dst, String *src);
extern "C" void  String_ctor_cstr(void *s, const char *text, bool copy);
unsigned int GameText_getLanguage();
extern "C" void  String_ctor_default(void *s);

// ---- setVisible_1693a4.cpp ----
void TouchButton::setVisible(bool value) {
    TouchButton *self = this;
    UC(self, 0xb2) = value;
}

// ---- setPosition_1692a8.cpp ----
void TouchButton::setPosition(int x, int y, unsigned char flags) {
    TouchButton *self = this;
    unsigned int f = (unsigned int)flags;
    I(self, 0x78) = x;
    I(self, 0x7c) = y;
    UC(self, 0x74) = flags;
    if ((flags & 0x20) != 0) {
        y = y - I(self, 0x8c);
        I(self, 0x7c) = y;
    }
    if ((int)(f << 0x1e) < 0) {
        x = x - I(self, 0x90);
        I(self, 0x78) = x;
    }
    if ((int)(f << 0x19) < 0) {
        I(self, 0x7c) = y - I(self, 0x8c) / 2;
    }
    if ((int)(f << 0x1d) < 0) {
        I(self, 0x78) = x - I(self, 0x90) / 2;
    }
}

// ---- translate_1693b0.cpp ----
void TouchButton::translate(int dx, int dy) {
    TouchButton *self = this;
    I(self, 0x78) = dx + I(self, 0x78);
    I(self, 0x7c) = dy + I(self, 0x7c);
}

// ---- setNumberText_1692f8.cpp ----
void TouchButton::setNumberText(String *value) {
    TouchButton *self = this;
    return TB_assignString((char *)self + 0x2c, value);
}

// ---- _TouchButton_169188.cpp ----
// TouchButton::~TouchButton() — real C++ destructor so the demangled symbol contains "~TouchButton".

typedef unsigned int uint32_t;

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

void TouchButton::dtor() {
    TouchButton *self = this;
    dtor_fn d = gStringDtor;
    d((char *)self + 0x2c);
    d((char *)self + 0x18);
    d((char *)self + 0xc);
}

// ---- OnTouchMove_169a28.cpp ----
unsigned int TouchButton::OnTouchMove(int px, int py) {
    TouchButton *self = this;
    if (UC(self, 0xb2) != 0 && UC(self, 0xb3) == 0) {
        unsigned int r;
        if (UC(self, 0xb0) == 0)
            r = 0;
        else
            r = ((TouchButton *)(self))->touchedInside(px, py);
        UC(self, 0xb0) = (unsigned char)r;
        return r;
    }
    return 0;
}

// ---- isVisible_1693aa.cpp ----
uint8_t TouchButton::isVisible() {
    TouchButton *self = this;
    return UC(self, 0xb2);
}

// ---- setPressProgress_1693ca.cpp ----
void TouchButton::setPressProgress(float value) {
    TouchButton *self = this;
    F<float>(self, 0xbc) = value;
}

// ---- isTouched_1699d0.cpp ----
uint8_t TouchButton::isTouched() {
    TouchButton *self = this;
    return UC(self, 0xb0);
}

// ---- getText_169352.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the text String (stored at offset 0xc) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
// `struct RetStr` is provided by gof2/TouchButton.h (single shared definition).

RetStr TouchButton::getText() {
    TouchButton *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0xc), false);
    return r;
}

// ---- replaceTextKeepSize_169304.cpp ----
// String::operator=
// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas2;

void TouchButton::replaceTextKeepSize(String *text) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->assign(text);
    if (I(self, 0x70) == 10) {
        int w = I(self, 0x90);
        int tw = ((PaintCanvas*)(*g_TB_canvas2))->GetTextWidth((unsigned int)(U(self,8)), (void*)(text));
        I(self, 0xa4) = w / 2 - tw / 2;
    }
}

// ---- setSplitText_16934c.cpp ----
void TouchButton::setSplitText(String *value) {
    TouchButton *self = this;
    return TB_assignString((char *)self + 0x18, value);
}

// ---- getPosition_169380.cpp ----
struct Vec3 { float x, y, z; };

void TouchButton_getPosition(Vec3 *out, TouchButton *self)
{
    float x = (float)I(self, 0x78);
    float y = (float)I(self, 0x7c);
    out->z = 0;
    out->y = y;
    out->x = x;
}

// ---- setTextColor_1693c4.cpp ----
void TouchButton::setTextColor(int color) {
    TouchButton *self = this;
    I(self, 0xac) = color;
}

// ---- setAlwaysPressed_1693be.cpp ----
void TouchButton::setAlwaysPressed(bool value) {
    TouchButton *self = this;
    UC(self, 0xb1) = value;
}

// ---- resetTouch_1699c8.cpp ----
void TouchButton::resetTouch() {
    TouchButton *self = this;
    UC(self, 0xb0) = 0;
}

// ---- OnTouchBegin_1699d8.cpp ----
// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

bool TouchButton::OnTouchBegin(int px, int py) {
    TouchButton *self = this;
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0)
        return false;
    int r = ((TouchButton *)(self))->touchedInside(px, py);
    UC(self, 0xb0) = (unsigned char)r;
    if (r == 0)
        return false;
    ((FModSound *)(*g_TB_sound))->play(0x7c, 0, 0, 0.0f);
    return UC(self, 0xb0) != 0;
}

// ---- OnTouchEnd_169a54.cpp ----
// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

unsigned int TouchButton::OnTouchEnd(int px, int py) {
    TouchButton *self = this;
    unsigned int res;
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0) {
        res = 0;
    } else if (UC(self, 0xb0) == 0 || ((TouchButton *)(self))->touchedInside(px, py) == 0) {
        res = 0;
        UC(self, 0xb0) = 0;
    } else {
        UC(self, 0xb0) = 0;
        ((FModSound *)(*g_TB_sound))->play(0x7b, 0, 0, 0.0f);
        res = 1;
    }
    return res;
}

// ---- getWidth_169360.cpp ----
int TouchButton::getWidth() {
    TouchButton *self = this;
    return I(self, 0x90);
}

// ---- setPosition_16936c.cpp ----
void TouchButton::setPosition2(int x, int y) {
    TouchButton *self = this;
    return ((TouchButton *)(self))->setPosition(x, y, UC(self, 0x74));
}

// ---- setHalfTransparent_16939e.cpp ----
void TouchButton::setHalfTransparent(bool value) {
    TouchButton *self = this;
    UC(self, 0xb3) = value;
}

// ---- setPressProgressHighlight_1693d0.cpp ----
void TouchButton::setPressProgressHighlight(bool value) {
    TouchButton *self = this;
    UC(self, 0xb8) = value;
}

// ---- getHeight_169366.cpp ----
int TouchButton::getHeight() {
    TouchButton *self = this;
    return I(self, 0x88);
}

// ---- init_168598.cpp ----
// TouchButton::init(String const& text, unsigned int kind, int achId, int achStage,
//                   int x, int y, int width, unsigned char flags0, unsigned char flags1)
//
// Shared initialiser for every TouchButton flavour. It records geometry and
// flags, assigns the label text, then -- driven by `kind` -- creates the
// 2D images that make up the button skin (normal / pressed / disabled, plus the
// 9-patch background frame for menu buttons) and computes text/icon offsets.
// Finally it lays the button out via setPosition().
//
// The image-id lookup tables are fixed read-only data in the binary; they are
// reached through hidden helpers that take the row index.

// String::operator=
extern "C" void  String_assign_cstr_tmp(void *dst, const char *lit);    // assign from literal via temp


// Read-only id/frame tables, indexed by row. Hidden -> single pc-rel load.
extern "C" unsigned int   TB_iconTexId(int eliteVariant, int stage);     // -> 0xb4 texture id
extern "C" unsigned short TB_iconImgId(int eliteVariant, int stage);     // image for icon create
extern "C" unsigned short TB_medalSmallId(int achId);                    // small medal overlay
extern "C" unsigned short TB_frameId(int useAltSkin, unsigned int kind, int slot); // 9-patch frame ids

// Singletons (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas;
__attribute__((visibility("hidden"))) extern void **g_TB_achievements;
__attribute__((visibility("hidden"))) extern char  *g_TB_useAltSkin;     // bool: alternate skin table
__attribute__((visibility("hidden"))) extern char  *g_TB_langWide;       // bool: wide-language toggle
__attribute__((visibility("hidden"))) extern char  *g_TB_langWide2;      // bool: second toggle
__attribute__((visibility("hidden"))) extern void **g_TB_layoutMetrics;  // Layout providing +0x80 / +0x30
__attribute__((visibility("hidden"))) extern const char g_TB_emptyStr[];

// Helper: GetImage2DWidth/Height of the just-created image, then derive the
// centred label X offset. Used by several of the simple image-only kinds.
static void tb_basicImageGeometry(TouchButton *self, void *canvas)
{
    I(self, 0x88) = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
    int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
    I(self, 0x8c) = I(self, 0x88);
    I(self, 0x90) = w;
    I(self, 0x94) = w;
    int tw = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
    int h = I(self, 0x88);
    I(self, 0xa4) = w / 2 - tw / 2;
    int th = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
    I(self, 0xa8) = h / 2 - th / 2;
}

int TouchButton::init(String *text, unsigned int kind, int achId, int achStage, int width, int d_unused, int x, int y, unsigned char flags0, unsigned char flags1) {
    TouchButton *self = this;
    void *canvas = *g_TB_canvas;

    I(self, 0x70)  = (int)kind;
    US(self, 0xb2) = 1;
    ((String *)((char *)self + 0xc))->assign(text);
    I(self, 0x24)  = achStage;          // generic "second image / sub-id" slot (in_r2)
    I(self, 0xac)  = -1;
    I(self, 0x6c)  = width;
    I(self, 0x0)   = 0;
    I(self, 0x4)   = 0;
    UC(self, 0x75) = flags1;
    UC(self, 0x74) = flags0;
    US(self, 0xb0) = 0;
    I(self, 0xa4)  = 0;
    I(self, 0xa8)  = 0;
    I(self, 0x94)  = 0;
    I(self, 0x98)  = 0;
    I(self, 0x9c)  = 0;
    UC(self, 0xb8) = 1;
    I(self, 0xbc)  = 0;
    I(self, 0xc0)  = 0;
    I(self, 0x88)  = 0;
    I(self, 0x80)  = x;
    I(self, 0x84)  = y;

    String_assign_cstr_tmp((char *)self + 0x2c, g_TB_emptyStr);
    String_assign_cstr_tmp((char *)self + 0x18, g_TB_emptyStr);
    I(self, 0x38) = -1;

    switch (kind) {
    case 4: {   // medal / achievement button
        void *ach = *g_TB_achievements;
        int elite = (((Achievements *)(ach))->isEliteMedal(achId) != 0) ? 1 : 0;
        U(self, 0xb4) = TB_iconTexId(elite, achStage);
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_iconImgId(elite, achStage)),(unsigned int*)(&U(self, 0x60)));
        I(self, 0x88) = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        I(self, 0xa8) = I(self, 0x88) + 5;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)((String *)((char *)self + 8)));
        I(self, 0x64) = -1;
        I(self, 0xa4) = w / 2 - tw / 2;
        I(self, 0x68) = -1;
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_medalSmallId(achId)),(unsigned int*)(&U(self, 0x68)));
        if (achStage != 0 || ((Achievements *)(ach))->isEliteMedal(achId) != 0)
            ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x96c),(unsigned int*)(&U(self, 0x64)));
        break;
    }
    case 10: {  // toggle-style button with a 0x2329 background
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(9000),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x2329),(unsigned int*)(&U(self, 0x3c)));
        I(self, 0x54) = I(self, 0x3c);
        I(self, 0x88) = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)((String *)((char *)self + 8)));
        I(self, 0xa4) = w / 2 - tw / 2;
        I(self, 0xa8) = ((PaintCanvas*)(canvas))->GetTextHeight((unsigned int)(U(self,8)));
        I(self, 0xc0) = I(*g_TB_layoutMetrics, 0x80);
        break;
    }
    case 0xc:   // simple two-image button (0x472 / 0x473)
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x472),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x473),(unsigned int*)(&U(self, 0x3c)));
        tb_basicImageGeometry(self, canvas);
        break;
    case 0xd:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x517),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x518),(unsigned int*)(&U(self, 0x3c)));
        I(self, 0x54) = I(self, 0x3c);
        tb_basicImageGeometry(self, canvas);
        break;
    case 0xe:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x53c),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x53b),(unsigned int*)(&U(self, 0x3c)));
        goto wide_text_layout;
    case 0xf:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x53e),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x53d),(unsigned int*)(&U(self, 0x3c)));
        goto wide_text_layout;
    case 0x10:  // background supplied by caller at 0x48
        I(self, 0x48) = achStage;
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0xbb9),(unsigned int*)(&U(self, 0x3c)));
        tb_basicImageGeometry(self, canvas);
        break;
    case 0x11:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0xbc0),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0xbc1),(unsigned int*)(&U(self, 0x3c)));
        goto wide_text_layout;
    case 0x12:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0xbc0),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0xbc1),(unsigned int*)(&U(self, 0x3c)));
        goto wide_text_layout;
    case 0x14:
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x1f6e),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(0x1f6f),(unsigned int*)(&U(self, 0x3c)));
        goto wide_text_layout;
    case 0x13:  // pre-supplied images at 0x48 (caller) and 0x28 (ctor variant)
        I(self, 0x48) = achStage;
        I(self, 0x3c) = I(self, 0x28);
        tb_basicImageGeometry(self, canvas);
        break;
    default: {  // generic menu button: 9-patch frame from the skin table
        int alt = (*g_TB_useAltSkin != 0) ? 1 : 0;
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 0)),(unsigned int*)(&U(self, 0x48)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 1)),(unsigned int*)(&U(self, 0x4c)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 2)),(unsigned int*)(&U(self, 0x50)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 3)),(unsigned int*)(&U(self, 0x3c)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 4)),(unsigned int*)(&U(self, 0x40)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 5)),(unsigned int*)(&U(self, 0x44)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 6)),(unsigned int*)(&U(self, 0x54)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 7)),(unsigned int*)(&U(self, 0x58)));
        ((PaintCanvas*)(canvas))->Image2DCreate((unsigned short)(TB_frameId(alt, kind, 8)),(unsigned int*)(&U(self, 0x5c)));

        I(self, 0x88) = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        I(self, 0x94) = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);   // left frame width
        I(self, 0x98) = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);   // mid frame width
        int rightW = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        I(self, 0x9c) = rightW;
        // height: kind 0xb uses the raw image height; otherwise a layout default
        if (kind != 0xb)
            I(self, 0x8c) = I((char *)*g_TB_layoutMetrics + 0x30, 0);
        else
            I(self, 0x8c) = I(self, 0x88);

        if (kind < 7 && ((1 << (kind & 0xff)) & 0x61) != 0) {
            I(self, 0x9c) = rightW - 2;
        } else if ((kind - 7) < 3 && *g_TB_useAltSkin != 0) {
            int hh;
            if (*g_TB_langWide != 0)
                hh = 0x46;
            else
                hh = (*g_TB_langWide2 != 0) ? 0x64 : 0x32;
            I(self, 0x8c) = hh;
        }
        ((TouchButton *)(self))->setText((String *)((char *)self + 8));
        break;
    }
    }

    goto done;

wide_text_layout: {
        // Shared epilogue for the 'arrow' style kinds (0xe/0xf/0x11/0x12/0x14):
        // a language-dependent vertical text factor is applied.
        unsigned int lang = GameText_getLanguage();
        float factor;
        if ((lang & 0xffff) < 0x10 && ((1 << (lang & 0xff)) & 0x8c00) != 0)
            factor = 1.0f;
        else
            factor = (lang == 0xe) ? 1.0f : 1.5f;

        I(self, 0x54) = I(self, 0x3c);
        I(self, 0x88) = ((PaintCanvas*)(canvas))->GetImage2DHeight(0);
        int w = ((PaintCanvas*)(canvas))->GetImage2DWidth(0);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        int tw = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)((String *)((char *)self + 8)));
        int h = I(self, 0x88);
        I(self, 0xa4) = w / 2 - tw / 2;
        int th = ((PaintCanvas*)(canvas))->GetTextHeight((unsigned int)(U(self,8)));
        I(self, 0xa8) = (int)((float)(h / 2) + factor * (float)th);
        I(self, 0xc0) = I(*g_TB_layoutMetrics, 0x80);
    }

done:
    ((TouchButton *)(self))->setPosition(x, y, flags0);
    return 0;
}

// ---- TouchButton_168ffc.cpp ----
// TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char)
//
// Same shape as the other constructors but the label text comes from an empty
// string literal rather than a caller-supplied String. The font spacing is the
// engine default and the kerning is sampled from the current font.

extern "C" void  String_ctor_default(void *s);                 // String::String()

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;   // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing; // default glyph spacing
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];   // ""

void TouchButton_168ffc(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, int d,
                                   unsigned char flags0, unsigned char flags1)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = ((PaintCanvas*)(canvas))->FontGetSpacing((unsigned int)(U(self,8)));

    unsigned char tmp[12];       // String is a 12-byte value type (ctor/dtor are engine calls)
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    ((TouchButton *)(self))->init((String *)tmp, kind, a, b, c, d, -1, -1, flags0, flags1);
    ((String *)(tmp))->dtor();
}

// ---- draw_1693d8.cpp ----
// TouchButton::draw()
//   Renders the button for its current state. Saves the canvas colour and font
//   spacing, draws the appropriate skin (single icon, 9-patch frame + label, a
//   progress fill, or a plain image) depending on `kind` (offset 0x70) and the
//   pressed/highlight flags, then restores spacing and colour.

// Singletons (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_d_canvas;
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutA;   // pressed/disabled tint layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutBG;  // background-pattern layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutC;   // progress-fill layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutEnd; // final restore layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_unitStr;   // "%"-style unit String
__attribute__((visibility("hidden"))) extern unsigned int g_TB_d_frameMask; // kinds that get a frame

void TouchButton::draw() {
    TouchButton *self = this;
    void *canvas = *g_TB_d_canvas;
    unsigned int savedColor = ((PaintCanvas*)(canvas))->GetColor();

    if (C(self, 0xb2) == 0)
        return;

    if (C(self, 0xb3) != 0) {
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffff2f);   // mvn #0xd0 == 0xffffff2f
        ((Layout *)(*g_TB_d_layoutA))->setDrawColor(-0xd1);
    } else {
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
    }

    short savedSpacing = (short)((PaintCanvas*)(canvas))->FontGetSpacing((unsigned int)(U(self,8)));
    ((PaintCanvas*)(canvas))->FontSetSpacing((unsigned int)(U(self, 0x8)),(short)((short)I(self, 0xc4)));

    unsigned int kind = U(self, 0x70);
    int icon = -1;       // image to draw at the tail (offset 0x78 position)
    int iconY = 0;
    bool tailIcon = false;

    if (kind == 0x10) {
        ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(I(self, 0x48)),(int)(I(self, 0x78)),(int)(I(self, 0x7c)));
        if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0) {
            icon = I(self, 0x3c);
            iconY = I(self, 0x78);
            tailIcon = true;
        }
    } else if (kind == 4) {
        ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(I(self, 0x60)),(int)(I(self, 0x78)),(int)(I(self, 0x7c)));
        if (I(self, 0x68) != -1) {
            ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
            ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(I(self, 0x68)),(int)(I(self, 0x78) + (I(self, 0x90) >> 1)),(int)((I(self, 0x7c) + (I(self, 0x88) >> 1)) - 1),(unsigned char)(0x11),(unsigned char)(0x44));
            ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
            if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0)
                ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(I(self, 0x64)),(int)(I(self, 0x78)),(int)(I(self, 0x7c)));
        }
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
        ((PaintCanvas*)(canvas))->DrawString((unsigned int)(U(self,8)),(void*)((char *)self + 0xc),(int)(I(self, 0x78) + I(self, 0xa4)),(int)(I(self, 0x7c) + I(self, 0xa8)),false);
    } else {
        // generic frame / label kinds.
        int base;
        if (C(self, 0xb0) != 0)
            base = I(self, 0x3c);
        else if (C(self, 0xb1) != 0)
            base = I(self, 0x54);
        else
            base = I(self, 0x48);

        if (kind <= 0x14 && ((1u << (kind & 0xff)) & g_TB_d_frameMask) != 0) {
            // draw the 9-patch background frame + middle stretch.
            unsigned int frameLeft;
            int frameMid;
            if (C(self, 0xb0) != 0) {
                frameLeft = U(self, 0x40);
                frameMid  = I(self, 0x44);
            } else if (C(self, 0xb1) != 0) {
                frameLeft = U(self, 0x58);
                frameMid  = I(self, 0x5c);
            } else {
                frameLeft = U(self, 0x4c);
                frameMid  = I(self, 0x50);
            }
            ((Layout *)(*g_TB_d_layoutBG))->drawBGPattern(frameLeft, I(self, 0x94) + I(self, 0x78), I(self, 0x7c), I(self, 0xa0), I(self, 0x88));
            ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(frameMid),(int)(I(self, 0x78) + I(self, 0x94) + I(self, 0xa0)),(int)(I(self, 0x7c)));
        }
        ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(base),(int)(I(self, 0x78)),(int)(I(self, 0x7c)));

        void *layoutC = *g_TB_d_layoutC;
        ((Layout *)(layoutC))->setDrawColor(-1);

        // optional progress fill (when 0xbc > 0).
        float prog = F<float>(self, 0xbc);
        if (prog > 0.0f) {
            ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
            ((Layout *)(layoutC))->setDrawColor(-0x80);
            int span = I(self, 0x94);
            int total = I(self, 0x9c) + I(self, 0xa0) + span;
            int filled = (int)(prog * (float)total);
            int leftImg = (C(self, 0xb8) == 0) ? I(self, 0x48) : I(self, 0x3c);
            int drawW = (filled < span) ? filled : span;
            ((PaintCanvas*)(canvas))->DrawRegion2D((unsigned int)((unsigned int)leftImg),(int)(0),(int)(0),(int)(drawW),(int)(I(self, 0x88)),(float)(filled),(int)(0),(int)(0),(int)(0),(int)(I(self, 0x78)));

            int mid = I(self, 0x94);
            if (mid < filled) {
                int midImg = (C(self, 0xb8) == 0) ? I(self, 0x4c) : I(self, 0x40);
                int patW = I(self, 0xa0);
                if (filled - mid < patW) patW = filled - mid;
                ((Layout *)(layoutC))->drawBGPattern((unsigned int)midImg, mid + I(self, 0x78), I(self, 0x7c), patW, I(self, 0x88));
                mid = I(self, 0x94);
            }
            int rstart = I(self, 0xa0) + mid;
            if (rstart < filled) {
                int rImg = (C(self, 0xb8) == 0) ? I(self, 0x50) : I(self, 0x44);
                int rW = (filled - mid) - I(self, 0xa0);
                if (I(self, 0x9c) < rW) rW = I(self, 0x9c);
                ((PaintCanvas*)(canvas))->DrawRegion2D((unsigned int)((unsigned int)rImg),(int)(0),(int)(0),(int)(rW),(int)(I(self, 0x88)),(float)(filled),(int)(0),(int)(0),(int)(0),(int)(rstart + I(self, 0x78)));
            }
            ((Layout *)(layoutC))->setDrawColor(-1);
        }

        // label colour: tinted when disabled (0xb3).
        unsigned int lblColor = U(self, 0xac);
        if (C(self, 0xb3) != 0)
            ((PaintCanvas*)g_PaintCanvas)->SetColor((unsigned char)((unsigned char)(lblColor >> 16)),(unsigned char)((unsigned char)(lblColor >> 8)),(unsigned char)((unsigned char)lblColor),(unsigned char)((unsigned char)(lblColor >> 24)));
        else
            ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);

        if (I(self, 0x24) == -1) {
            // primary label
            ((PaintCanvas*)(canvas))->DrawString((unsigned int)(U(self,8)),(void*)((char *)self + 0xc),(int)(I(self, 0x78) + I(self, 0xa4)),(int)(I(self, 0x7c) + I(self, 0xa8)),false);

            // secondary / value label (offset 0x18) when 0x20 set.
            if (I(self, 0x20) != 0) {
                String *t = (String *)((char *)self + 8);
                int px = I(self, 0x78);
                int w  = I(self, 0x90);
                int tx, ty;
                if (I(self, 0x70) == 10) {
                    int tw = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)(t));
                    int th = ((PaintCanvas*)(canvas))->GetTextHeight((unsigned int)(U(self,8)));
                    ty = I(self, 0x7c) + I(self, 0x88) + th * -2;
                    tx = (px + w / 2) - tw / 2;
                } else {
                    int off = I(self, 0xa4);
                    int tw  = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)(t));
                    ty = I(self, 0x7c) + I(self, 0xa8);
                    tx = ((w + px) - off) - tw;
                }
                ((PaintCanvas*)(canvas))->DrawString((unsigned int)(U(self,8)),(void*)((char *)self + 0x18),(int)(tx),(int)(ty),false);
            }

            // shortcut / corner label (offset 0x2c) when 0x34 set.
            if (I(self, 0x34) != 0) {
                ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
                String *u = (String *)(*g_TB_d_unitStr);
                int tw = ((PaintCanvas*)(canvas))->GetTextWidth((unsigned int)(U(self,8)),(void*)(u));
                ((PaintCanvas*)(canvas))->DrawString((unsigned int)(U(self,8)),(void*)((char *)self + 0x2c),(int)((I(self, 0x94) + I(self, 0x78)) - tw),(int)(I(self, 0x7c) + I(self, 0xa8)),false);
                ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
            }

            // small adornment image (offset 0x38) when set.
            if (I(self, 0x38) != -1) {
                ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
                ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(I(self, 0x38)),(int)((I(self, 0x78) + I(self, 0x90) + 6) - I(self, 0x94)),(int)(I(self, 0x7c) + 1),(unsigned char)(0x11),(unsigned char)(0x14));
                ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
            }
        } else if (I(self, 0x70) != 0x13) {
            icon = base;
            iconY = I(self, 0xa4) + I(self, 0x78);
            tailIcon = true;
        }
    }

    if (tailIcon)
        ((PaintCanvas*)(canvas))->DrawImage2D((unsigned int)(icon),(int)(I(self, 0x78) + I(self, 0xa4) + 0),(int)(iconY));

    ((Layout *)(*g_TB_d_layoutEnd))->setDrawColor(-1);
    ((PaintCanvas*)(canvas))->FontSetSpacing((unsigned int)(U(self, 0x8)),(short)(savedSpacing));
    ((PaintCanvas*)g_PaintCanvas)->SetColor(savedColor);
}

// ---- TouchButton_168e88.cpp ----
// TouchButton::TouchButton(String const& text, int type, int x, int y, int p5, uchar p6, uchar p7)
// Default-constructs the three embedded label Strings (+0xc/+0x18/+0x2c), seeds the font color
// (+0x08) from the active-font global, caches the current font spacing (+0xc4), then delegates
// to init() which builds the button's images and layout.

__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

TouchButton * TouchButton::ctor7(String *text, int type, int x, int y, int p5, unsigned char p6, unsigned char p7) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->ctor();
    ((String *)((char *)self + 0x18))->ctor();
    ((String *)((char *)self + 0x2c))->ctor();
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = ((PaintCanvas*)(*g_TB_c2))->FontGetSpacing((unsigned int)(U(self,8)));
    ((TouchButton *)(self))->init(text, (unsigned int)type, x, y, p5, 0, 0, 0, p6, p7);
    return self;
}

// ---- setText_1691b0.cpp ----
// String::operator=
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TB_canvas3;

void TouchButton::setText(String *text) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->assign(text);
    void **holder = g_TB_canvas3;
    int w = ((PaintCanvas*)(*holder))->GetTextWidth((unsigned int)(U(self,8)),(void*)(text));
    if (I(self, 0x24) != -1)
        w = ((PaintCanvas*)(*holder))->GetImage2DWidth(0);
    int a94 = I(self, 0x94);
    int x;
    if (I(self, 0x6c) < 1)
        x = w;
    else
        x = (I(self, 0x6c) - a94) - I(self, 0x9c);
    I(self, 0xa0) = x;
    x = I(self, 0x9c) + x + a94;
    I(self, 0x90) = x;
    unsigned char fl = UC(self, 0x75);
    if ((fl & 2) == 0) {
        if ((fl & 1) != 0) {
            I(self, 0xa4) = a94;
            goto height;
        }
        x = (x - w) / 2;
        I(self, 0xa4) = x;
        if (I(self, 0x70) == 6) {
            x = x + -5;
        } else {
            if (I(self, 0x70) != 5)
                goto height;
            x = x + 5;
        }
    } else {
        x = x - (w + a94);
    }
    I(self, 0xa4) = x;
height:
    int h = I(self, 0x88);
    int th = ((PaintCanvas*)(*holder))->GetTextHeight((unsigned int)(U(self,8)));
    th = (h - th) / 2;
    I(self, 0xa8) = th;
    if (I(self, 0x70) == 3)
        I(self, 0xa8) = th + 2;
    if (I(self, 0x24) != -1) {
        h = I(self, 0x88);
        int ih = ((PaintCanvas*)(*holder))->GetImage2DHeight(0);
        I(self, 0xa8) = (h - ih) / 2;
        if (I(self, 0x70) == 1)
            I(self, 0xa4) = I(self, 0xa4) + 3;
    }
    return ((TouchButton *)(self))->setPosition(I(self, 0x80), I(self, 0x84), UC(self, 0x74));
}

// ---- setYPosition_169374.cpp ----
void TouchButton::setYPosition(int y) {
    TouchButton *self = this;
    return ((TouchButton *)(self))->setPosition(I(self, 0x78), y, UC(self, 0x74));
}

// ---- touchedInside_169938.cpp ----
// Singleton pointer dereferenced twice (PaintCanvas-like). Hidden -> single pc-rel load.

bool TouchButton::touchedInside(int px, int py) {
    TouchButton *self = this;
    int x = I(self, 0x78);
    int xm1 = x - 1;
    int h;
    int top;
    if (I(self, 0x70) == 3) {
        int v = ((int *)*g_TB_canvas)[0x38 / 4];
        if (xm1 + v > px)
            return false;
        if (I(self, 0x90) + ((x + 1) - v) <= px)
            return false;
        top = I(self, 0x7c);
        bool r = false;
        if (top - 1 > py)
            return r;
        h = I(self, 0x8c);
    } else {
        int m = I(self, 0xc0);
        if (xm1 - m > px)
            return false;
        if (I(self, 0x90) + (x + m) + 1 <= px)
            return false;
        top = I(self, 0x7c) + ~m;
        bool r = false;
        if (top > py)
            return r;
        h = I(self, 0x8c);
        top = I(self, 0x7c) + m;
    }
    return h + top + 1 >= py;
}

// ---- TouchButton_168d9c.cpp ----
// TouchButton::TouchButton(unsigned int, unsigned int, int, int, int, unsigned char)
//
// Variant that also stashes the second unsigned argument at offset 0x28 (used
// later as a pre-supplied image handle, see init() case 0x13). Otherwise the
// usual three-String construction + spacing save + init().


__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing;
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];

void TouchButton_168d9c(TouchButton *self, unsigned int kind, unsigned int image,
                                   int a, int b, int c, unsigned char flag)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x28) = image;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = ((PaintCanvas*)(canvas))->FontGetSpacing((unsigned int)(U(self,8)));

    unsigned char tmp[12];       // String is a 12-byte value type
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    ((TouchButton *)(self))->init((String *)tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
    ((String *)(tmp))->dtor();
}

// ---- TouchButton_1684f0.cpp ----
// TouchButton::TouchButton(String const& text, int x, int y, int p4, uchar p5)
// 5-arg overload: same setup as the other label ctors -- three embedded Strings, font color
// (+0x08), cached spacing (+0xc4) -- then init().

__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

TouchButton * TouchButton::ctor5(String *text, int x, int y, int p4, unsigned char p5) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->ctor();
    ((String *)((char *)self + 0x18))->ctor();
    ((String *)((char *)self + 0x2c))->ctor();
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = ((PaintCanvas*)(*g_TB_c2))->FontGetSpacing((unsigned int)(U(self,8)));
    ((TouchButton *)(self))->init(text, 0xffffffff, 4, x, y, p4, 0, 0, p5, 0x44);
    return self;
}

// ---- TouchButton_168f30.cpp ----
// TouchButton::TouchButton(String const&, int, int, int, int,
//                          unsigned char, unsigned char, unsigned int, int)
//
// Constructs the three embedded String members, temporarily overrides the
// shared font glyph spacing with this button's (spacing, kerning) pair, runs
// the common init(), then restores the previous spacing.

extern "C" void  String_ctor_default(void *s);                 // String::String()

// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;

TouchButton *TouchButton_168f30(TouchButton *self, String *text,
                                           int a, int b, int c, int d,
                                           unsigned char flags0, unsigned char flags1,
                                           unsigned int spacing, int kerning)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    I(self, 0xc4) = kerning;
    U(self, 0x8)  = spacing;

    void *canvas = *g_TB_canvas_ctor;
    short prev = ((PaintCanvas*)(canvas))->FontGetSpacing((unsigned int)(U(self,8)));
    ((PaintCanvas*)(canvas))->FontSetSpacing((unsigned int)(spacing),(short)((short)kerning));

    ((TouchButton *)(self))->init(text, spacing, a, b, c, d, -1, -1, flags0, flags1);

    ((PaintCanvas*)(canvas))->FontSetSpacing((unsigned int)(spacing),(short)(prev));
    return self;
}

// ---- TouchButton_1690e4.cpp ----
// AbyssEngine::String::String()
// Two singletons (cell value = holder; one deref yields value/object).
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

TouchButton * TouchButton::ctor6(int x, int y, String *text, int p4, int p5, unsigned char p6) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->ctor();
    ((String *)((char *)self + 0x18))->ctor();
    ((String *)((char *)self + 0x2c))->ctor();
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = ((PaintCanvas*)(*g_TB_c2))->FontGetSpacing((unsigned int)(U(self,8)));
    ((TouchButton *)(self))->init(text, 0xffffffff, 4, x, y, p4, p5, 0, p6, 0x44);
    return self;
}

// ---- TouchButton_168cb0.cpp ----
// TouchButton::TouchButton(unsigned int, int, int, int, unsigned char)
//
// Five-argument convenience constructor: kind + position + a single flag byte.
// Builds the three embedded Strings, samples the default spacing/kerning, then
// runs init() with an empty label and a fixed extra parameter (0x44).


__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing;
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];

void TouchButton_168cb0(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, unsigned char flag)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = ((PaintCanvas*)(canvas))->FontGetSpacing((unsigned int)(U(self,8)));

    unsigned char tmp[12];       // String is a 12-byte value type
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    ((TouchButton *)(self))->init((String *)tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
    ((String *)(tmp))->dtor();
}

// ---- TouchButton::TouchButton(String const& text, int type, int x, int y, int width, int icon, int style) ----
// Full label/menu-button constructor used by DialogueWindow / SpaceLounge / StarMap.
// Same prologue as the other label ctors (three embedded Strings, font colour at
// +0x08, cached glyph spacing at +0xc4), then delegates to the shared init().
// `icon` is the optional sub-image id (init's "achStage"/+0x24 slot); `style`
// becomes flags0 (+0x74). flags1 defaults to 0x44 like the other label ctors.
TouchButton * TouchButton::ctor(String *text, int type, int x, int y, int width, int icon, int style) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->ctor();
    ((String *)((char *)self + 0x18))->ctor();
    ((String *)((char *)self + 0x2c))->ctor();
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = ((PaintCanvas*)(*g_TB_c2))->FontGetSpacing((unsigned int)(U(self,8)));
    ((TouchButton *)(self))->init(text, (unsigned int)type, 0, icon, width, 0, x, y, (unsigned char)style, 0x44);
    return self;
}

// ---- TouchButton::TouchButton(String const& text, int type, int x, int y, int width, int icon, int mode) ----
// Identical construction to ctor() above (the WantedWindow call site reaches this
// via its own veneer); `mode` is the flags0 byte.
TouchButton * TouchButton::ctor8(String *text, int type, int x, int y, int width, int icon, int mode) {
    TouchButton *self = this;
    ((String *)((char *)self + 0xc))->ctor();
    ((String *)((char *)self + 0x18))->ctor();
    ((String *)((char *)self + 0x2c))->ctor();
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = ((PaintCanvas*)(*g_TB_c2))->FontGetSpacing((unsigned int)(U(self,8)));
    ((TouchButton *)(self))->init(text, (unsigned int)type, 0, icon, width, 0, x, y, (unsigned char)mode, 0x44);
    return self;
}

// ---- TouchButton::setPosition3(int x, int y, int align) ----
// Three-argument placement helper: positions the button using `align` as the
// anchor-flags byte (the same flags consumed by setPosition()).
void TouchButton::setPosition3(int x, int y, int align) {
    TouchButton *self = this;
    ((TouchButton *)(self))->setPosition(x, y, (unsigned char)align);
}

// ---- TouchButton::touch_end(int x, int y) ----
// Touch-release handler entry point: forwards to OnTouchEnd, which clears the
// pressed flag and reports whether the release counts as a click.
unsigned int TouchButton::touch_end(int x, int y) {
    TouchButton *self = this;
    return ((TouchButton *)(self))->OnTouchEnd(x, y);
}
