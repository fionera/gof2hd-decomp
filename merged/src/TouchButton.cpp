#include "TouchButton.h"


extern "C" void TB_assignString(void *dst, String *src);
extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" int PaintCanvas_GetTextWidth(void *canvas, void *str, String *s);
extern "C" void FModSound_play(void *snd, int id, void *a, void *b, float f);
extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);
extern "C" void  String_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  String_dtor(void *s);
extern "C" int   Achievements_isEliteMedal(void *ach, int id);
extern "C" void  PaintCanvas_Image2DCreate(void *canvas, unsigned short imgId, unsigned int *outHandle);
extern "C" int   PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int   PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" int   PaintCanvas_GetTextWidth(void *canvas, String *text);
extern "C" int   PaintCanvas_GetTextHeight(void *canvas);
extern "C" unsigned int GameText_getLanguage();
extern "C" void  TouchButton_setText(TouchButton *self, String *text);
extern "C" void  TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);
extern "C" int   PaintCanvas_FontGetSpacing(void *canvas);
extern "C" unsigned int PaintCanvas_GetColor(void *canvas);
extern "C" void  PaintCanvas_SetColor(unsigned int c);
extern "C" void  Layout_setDrawColor(void *layout, int color);
extern "C" void  PaintCanvas_FontSetSpacing(void *canvas, unsigned int spacing, short kerning);
extern "C" void  PaintCanvas_DrawImage2D(void *canvas, int img, int x, int y);
extern "C" int   Layout_drawBGPattern(void *layout, unsigned int img, int x, int y, int w, int h);
extern "C" void String_ctor(void *s);
extern "C" int  PaintCanvas_FontGetSpacing(void *canvas);
extern "C" int PaintCanvas_GetTextWidth2(void *canvas, void *s, String *t);
extern "C" int PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int PaintCanvas_GetTextHeight(void *canvas);
extern "C" int PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" void  String_ctor_default(void *s);
extern "C" short PaintCanvas_FontGetSpacing(void *canvas);
extern "C" int PaintCanvas_FontGetSpacing(void *canvas);

// ---- setVisible_1693a4.cpp ----
extern "C" void TouchButton_setVisible(TouchButton *self, bool value)
{
    UC(self, 0xb2) = value;
}

// ---- setPosition_1692a8.cpp ----
extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags)
{
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
extern "C" void TouchButton_translate(TouchButton *self, int dx, int dy)
{
    I(self, 0x78) = dx + I(self, 0x78);
    I(self, 0x7c) = dy + I(self, 0x7c);
}

// ---- setNumberText_1692f8.cpp ----
extern "C" void TouchButton_setNumberText(TouchButton *self, String *value)
{
    return TB_assignString((char *)self + 0x2c, value);
}

// ---- _TouchButton_169188.cpp ----
// TouchButton::~TouchButton() — real C++ destructor so the demangled symbol contains "~TouchButton".

typedef unsigned int uint32_t;

typedef void (*dtor_fn)(void *) __attribute__((nothrow));
extern dtor_fn const gStringDtor __attribute__((visibility("hidden")));

struct TouchButton {
    unsigned char pad[0x100];
    ~TouchButton() noexcept(false);
};

__attribute__((minsize)) TouchButton::~TouchButton() noexcept(false)
{
    dtor_fn d = gStringDtor;
    d((char *)this + 0x2c);
    d((char *)this + 0x18);
    d((char *)this + 0xc);
}

// ---- OnTouchMove_169a28.cpp ----
extern "C" unsigned int TouchButton_OnTouchMove(TouchButton *self, int px, int py)
{
    if (UC(self, 0xb2) != 0 && UC(self, 0xb3) == 0) {
        unsigned int r;
        if (UC(self, 0xb0) == 0)
            r = 0;
        else
            r = TouchButton_touchedInside(self, px, py);
        UC(self, 0xb0) = (unsigned char)r;
        return r;
    }
    return 0;
}

// ---- isVisible_1693aa.cpp ----
extern "C" uint8_t TouchButton_isVisible(TouchButton *self)
{
    return UC(self, 0xb2);
}

// ---- setPressProgress_1693ca.cpp ----
extern "C" void TouchButton_setPressProgress(TouchButton *self, float value)
{
    F(self, 0xbc) = value;
}

// ---- isTouched_1699d0.cpp ----
extern "C" uint8_t TouchButton_isTouched(TouchButton *self)
{
    return UC(self, 0xb0);
}

// ---- getText_169352.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void

// Returns the text String (stored at offset 0xc) by value. The copy-ctor returns
// void, so the compiler keeps a frame + restores the sret pointer (r0).
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr TouchButton_getText(TouchButton *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0xc, false);
    return r;
}

// ---- replaceTextKeepSize_169304.cpp ----
extern "C" void String_assign(void *dst, String *src);            // String::operator=
// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas2;

extern "C" void TouchButton_replaceTextKeepSize(TouchButton *self, String *text)
{
    String_assign((char *)self + 0xc, text);
    if (I(self, 0x70) == 10) {
        int w = I(self, 0x90);
        int tw = PaintCanvas_GetTextWidth(*g_TB_canvas2, P(self, 8), text);
        I(self, 0xa4) = w / 2 - tw / 2;
    }
}

// ---- setSplitText_16934c.cpp ----
extern "C" void TouchButton_setSplitText(TouchButton *self, String *value)
{
    return TB_assignString((char *)self + 0x18, value);
}

// ---- getPosition_169380.cpp ----
struct Vec3 { float x, y, z; };

extern "C" void TouchButton_getPosition(Vec3 *out, TouchButton *self)
{
    float x = (float)I(self, 0x78);
    float y = (float)I(self, 0x7c);
    out->z = 0;
    out->y = y;
    out->x = x;
}

// ---- setTextColor_1693c4.cpp ----
extern "C" void TouchButton_setTextColor(TouchButton *self, int color)
{
    I(self, 0xac) = color;
}

// ---- setAlwaysPressed_1693be.cpp ----
extern "C" void TouchButton_setAlwaysPressed(TouchButton *self, bool value)
{
    UC(self, 0xb1) = value;
}

// ---- resetTouch_1699c8.cpp ----
extern "C" void TouchButton_resetTouch(TouchButton *self)
{
    UC(self, 0xb0) = 0;
}

// ---- OnTouchBegin_1699d8.cpp ----
// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

extern "C" bool TouchButton_OnTouchBegin(TouchButton *self, int px, int py)
{
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0)
        return false;
    int r = TouchButton_touchedInside(self, px, py);
    UC(self, 0xb0) = (unsigned char)r;
    if (r == 0)
        return false;
    FModSound_play(*g_TB_sound, 0x7c, 0, 0, 0.0f);
    return UC(self, 0xb0) != 0;
}

// ---- OnTouchEnd_169a54.cpp ----
// FModSound singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_sound;

extern "C" unsigned int TouchButton_OnTouchEnd(TouchButton *self, int px, int py)
{
    unsigned int res;
    if (UC(self, 0xb2) == 0 || UC(self, 0xb3) != 0) {
        res = 0;
    } else if (UC(self, 0xb0) == 0 || TouchButton_touchedInside(self, px, py) == 0) {
        res = 0;
        UC(self, 0xb0) = 0;
    } else {
        UC(self, 0xb0) = 0;
        FModSound_play(*g_TB_sound, 0x7b, 0, 0, 0.0f);
        res = 1;
    }
    return res;
}

// ---- getWidth_169360.cpp ----
extern "C" int TouchButton_getWidth(TouchButton *self)
{
    return I(self, 0x90);
}

// ---- setPosition_16936c.cpp ----
extern "C" void TouchButton_setPosition2(TouchButton *self, int x, int y)
{
    return TouchButton_setPosition(self, x, y, UC(self, 0x74));
}

// ---- setHalfTransparent_16939e.cpp ----
extern "C" void TouchButton_setHalfTransparent(TouchButton *self, bool value)
{
    UC(self, 0xb3) = value;
}

// ---- setPressProgressHighlight_1693d0.cpp ----
extern "C" void TouchButton_setPressProgressHighlight(TouchButton *self, bool value)
{
    UC(self, 0xb8) = value;
}

// ---- getHeight_169366.cpp ----
extern "C" int TouchButton_getHeight(TouchButton *self)
{
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

extern "C" void  String_assign(void *dst, String *src);                 // String::operator=
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
    I(self, 0x88) = PaintCanvas_GetImage2DHeight(canvas);
    int w = PaintCanvas_GetImage2DWidth(canvas);
    I(self, 0x8c) = I(self, 0x88);
    I(self, 0x90) = w;
    I(self, 0x94) = w;
    int tw = PaintCanvas_GetImage2DWidth(canvas);
    int h = I(self, 0x88);
    I(self, 0xa4) = w / 2 - tw / 2;
    int th = PaintCanvas_GetImage2DHeight(canvas);
    I(self, 0xa8) = h / 2 - th / 2;
}

extern "C" int TouchButton_init(TouchButton *self, String *text,
                                unsigned int kind, int achId, int achStage,
                                int x, int y, int width,
                                unsigned char flags0, unsigned char flags1)
{
    void *canvas = *g_TB_canvas;

    I(self, 0x70)  = (int)kind;
    US(self, 0xb2) = 1;
    String_assign((char *)self + 0xc, text);
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
        int elite = (Achievements_isEliteMedal(ach, achId) != 0) ? 1 : 0;
        U(self, 0xb4) = TB_iconTexId(elite, achStage);
        PaintCanvas_Image2DCreate(canvas, TB_iconImgId(elite, achStage), &U(self, 0x60));
        I(self, 0x88) = PaintCanvas_GetImage2DHeight(canvas);
        int w = PaintCanvas_GetImage2DWidth(canvas);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        I(self, 0xa8) = I(self, 0x88) + 5;
        int tw = PaintCanvas_GetTextWidth(canvas, (String *)((char *)self + 8));
        I(self, 0x64) = -1;
        I(self, 0xa4) = w / 2 - tw / 2;
        I(self, 0x68) = -1;
        PaintCanvas_Image2DCreate(canvas, TB_medalSmallId(achId), &U(self, 0x68));
        if (achStage != 0 || Achievements_isEliteMedal(ach, achId) != 0)
            PaintCanvas_Image2DCreate(canvas, 0x96c, &U(self, 0x64));
        break;
    }
    case 10: {  // toggle-style button with a 0x2329 background
        PaintCanvas_Image2DCreate(canvas, 9000, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x2329, &U(self, 0x3c));
        I(self, 0x54) = I(self, 0x3c);
        I(self, 0x88) = PaintCanvas_GetImage2DHeight(canvas);
        int w = PaintCanvas_GetImage2DWidth(canvas);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        int tw = PaintCanvas_GetTextWidth(canvas, (String *)((char *)self + 8));
        I(self, 0xa4) = w / 2 - tw / 2;
        I(self, 0xa8) = PaintCanvas_GetTextHeight(canvas);
        I(self, 0xc0) = I(*g_TB_layoutMetrics, 0x80);
        break;
    }
    case 0xc:   // simple two-image button (0x472 / 0x473)
        PaintCanvas_Image2DCreate(canvas, 0x472, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x473, &U(self, 0x3c));
        tb_basicImageGeometry(self, canvas);
        break;
    case 0xd:
        PaintCanvas_Image2DCreate(canvas, 0x517, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x518, &U(self, 0x3c));
        I(self, 0x54) = I(self, 0x3c);
        tb_basicImageGeometry(self, canvas);
        break;
    case 0xe:
        PaintCanvas_Image2DCreate(canvas, 0x53c, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x53b, &U(self, 0x3c));
        goto wide_text_layout;
    case 0xf:
        PaintCanvas_Image2DCreate(canvas, 0x53e, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x53d, &U(self, 0x3c));
        goto wide_text_layout;
    case 0x10:  // background supplied by caller at 0x48
        I(self, 0x48) = achStage;
        PaintCanvas_Image2DCreate(canvas, 0xbb9, &U(self, 0x3c));
        tb_basicImageGeometry(self, canvas);
        break;
    case 0x11:
        PaintCanvas_Image2DCreate(canvas, 0xbc0, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0xbc1, &U(self, 0x3c));
        goto wide_text_layout;
    case 0x12:
        PaintCanvas_Image2DCreate(canvas, 0xbc0, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0xbc1, &U(self, 0x3c));
        goto wide_text_layout;
    case 0x14:
        PaintCanvas_Image2DCreate(canvas, 0x1f6e, &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, 0x1f6f, &U(self, 0x3c));
        goto wide_text_layout;
    case 0x13:  // pre-supplied images at 0x48 (caller) and 0x28 (ctor variant)
        I(self, 0x48) = achStage;
        I(self, 0x3c) = I(self, 0x28);
        tb_basicImageGeometry(self, canvas);
        break;
    default: {  // generic menu button: 9-patch frame from the skin table
        int alt = (*g_TB_useAltSkin != 0) ? 1 : 0;
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 0), &U(self, 0x48));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 1), &U(self, 0x4c));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 2), &U(self, 0x50));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 3), &U(self, 0x3c));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 4), &U(self, 0x40));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 5), &U(self, 0x44));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 6), &U(self, 0x54));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 7), &U(self, 0x58));
        PaintCanvas_Image2DCreate(canvas, TB_frameId(alt, kind, 8), &U(self, 0x5c));

        I(self, 0x88) = PaintCanvas_GetImage2DHeight(canvas);
        I(self, 0x94) = PaintCanvas_GetImage2DWidth(canvas);   // left frame width
        I(self, 0x98) = PaintCanvas_GetImage2DWidth(canvas);   // mid frame width
        int rightW = PaintCanvas_GetImage2DWidth(canvas);
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
        TouchButton_setText(self, (String *)((char *)self + 8));
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
        I(self, 0x88) = PaintCanvas_GetImage2DHeight(canvas);
        int w = PaintCanvas_GetImage2DWidth(canvas);
        I(self, 0x8c) = I(self, 0x88);
        I(self, 0x90) = w;
        I(self, 0x94) = w;
        int tw = PaintCanvas_GetTextWidth(canvas, (String *)((char *)self + 8));
        int h = I(self, 0x88);
        I(self, 0xa4) = w / 2 - tw / 2;
        int th = PaintCanvas_GetTextHeight(canvas);
        I(self, 0xa8) = (int)((float)(h / 2) + factor * (float)th);
        I(self, 0xc0) = I(*g_TB_layoutMetrics, 0x80);
    }

done:
    TouchButton_setPosition(self, x, y, flags0);
    return 0;
}

// ---- TouchButton_168ffc.cpp ----
// TouchButton::TouchButton(unsigned int, int, int, int, int, unsigned char, unsigned char)
//
// Same shape as the other constructors but the label text comes from an empty
// string literal rather than a caller-supplied String. The font spacing is the
// engine default and the kerning is sampled from the current font.

extern "C" void  String_ctor_default(void *s);                 // String::String()
extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;   // PaintCanvas singleton
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing; // default glyph spacing
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];   // ""

extern "C" void TouchButton_168ffc(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, int d,
                                   unsigned char flags0, unsigned char flags1)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(canvas);

    unsigned char tmp[12];       // String is a 12-byte value type (ctor/dtor are engine calls)
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    TouchButton_init(self, (String *)tmp, kind, a, b, c, d, -1, -1, flags0, flags1);
    String_dtor(tmp);
}

// ---- draw_1693d8.cpp ----
// TouchButton::draw()
//   Renders the button for its current state. Saves the canvas colour and font
//   spacing, draws the appropriate skin (single icon, 9-patch frame + label, a
//   progress fill, or a plain image) depending on `kind` (offset 0x70) and the
//   pressed/highlight flags, then restores spacing and colour.

extern "C" void  PaintCanvas_SetColorARGB(unsigned char a, unsigned char r,
                                          unsigned char g, unsigned char b);
extern "C" void  PaintCanvas_DrawImage2DEx(void *canvas, int img, int x, int y,
                                           int anchor, int flags);
extern "C" void  PaintCanvas_DrawString(void *canvas, String *text, void *posStr,
                                         int x, bool centered);
extern "C" int   PaintCanvas_DrawRegion2D(void *canvas, unsigned int img, int sx, int sy,
                                          int w, int h, int dw, int a, int b, int c, int y);

// Singletons (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_d_canvas;
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutA;   // pressed/disabled tint layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutBG;  // background-pattern layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutC;   // progress-fill layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_layoutEnd; // final restore layout
__attribute__((visibility("hidden"))) extern void **g_TB_d_unitStr;   // "%"-style unit String
__attribute__((visibility("hidden"))) extern unsigned int g_TB_d_frameMask; // kinds that get a frame

extern "C" void TouchButton_draw(TouchButton *self)
{
    void *canvas = *g_TB_d_canvas;
    unsigned int savedColor = PaintCanvas_GetColor(canvas);

    if (C(self, 0xb2) == 0)
        return;

    if (C(self, 0xb3) != 0) {
        PaintCanvas_SetColor(0xffffff2f);   // mvn #0xd0 == 0xffffff2f
        Layout_setDrawColor(*g_TB_d_layoutA, -0xd1);
    } else {
        PaintCanvas_SetColor(0xffffffff);
    }

    short savedSpacing = (short)PaintCanvas_FontGetSpacing(canvas);
    PaintCanvas_FontSetSpacing(canvas, U(self, 0x8), (short)I(self, 0xc4));

    unsigned int kind = U(self, 0x70);
    int icon = -1;       // image to draw at the tail (offset 0x78 position)
    int iconY = 0;
    bool tailIcon = false;

    if (kind == 0x10) {
        PaintCanvas_DrawImage2D(canvas, I(self, 0x48), I(self, 0x78), I(self, 0x7c));
        if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0) {
            icon = I(self, 0x3c);
            iconY = I(self, 0x78);
            tailIcon = true;
        }
    } else if (kind == 4) {
        PaintCanvas_DrawImage2D(canvas, I(self, 0x60), I(self, 0x78), I(self, 0x7c));
        if (I(self, 0x68) != -1) {
            PaintCanvas_SetColor(0xffffffff);
            PaintCanvas_DrawImage2DEx(canvas, I(self, 0x68),
                I(self, 0x78) + (I(self, 0x90) >> 1),
                (I(self, 0x7c) + (I(self, 0x88) >> 1)) - 1, 0x11, 0x44);
            PaintCanvas_SetColor(0xffffffff);
            if (C(self, 0xb0) != 0 || C(self, 0xb1) != 0)
                PaintCanvas_DrawImage2D(canvas, I(self, 0x64), I(self, 0x78), I(self, 0x7c));
        }
        PaintCanvas_SetColor(0xffffffff);
        PaintCanvas_DrawString(canvas, (String *)((char *)self + 8), (char *)self + 0xc,
            I(self, 0x78) + I(self, 0xa4), (bool)(I(self, 0x7c) + I(self, 0xa8)));
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
            Layout_drawBGPattern(*g_TB_d_layoutBG, frameLeft,
                I(self, 0x94) + I(self, 0x78), I(self, 0x7c), I(self, 0xa0), I(self, 0x88));
            PaintCanvas_DrawImage2D(canvas, frameMid,
                I(self, 0x78) + I(self, 0x94) + I(self, 0xa0), I(self, 0x7c));
        }
        PaintCanvas_DrawImage2D(canvas, base, I(self, 0x78), I(self, 0x7c));

        void *layoutC = *g_TB_d_layoutC;
        Layout_setDrawColor(layoutC, -1);

        // optional progress fill (when 0xbc > 0).
        float prog = F(self, 0xbc);
        if (prog > 0.0f) {
            PaintCanvas_SetColor(0xffffffff);
            Layout_setDrawColor(layoutC, -0x80);
            int span = I(self, 0x94);
            int total = I(self, 0x9c) + I(self, 0xa0) + span;
            int filled = (int)(prog * (float)total);
            int leftImg = (C(self, 0xb8) == 0) ? I(self, 0x48) : I(self, 0x3c);
            int drawW = (filled < span) ? filled : span;
            PaintCanvas_DrawRegion2D(canvas, (unsigned int)leftImg, 0, 0, drawW,
                I(self, 0x88), filled, 0, 0, 0, I(self, 0x78));

            int mid = I(self, 0x94);
            if (mid < filled) {
                int midImg = (C(self, 0xb8) == 0) ? I(self, 0x4c) : I(self, 0x40);
                int patW = I(self, 0xa0);
                if (filled - mid < patW) patW = filled - mid;
                Layout_drawBGPattern(layoutC, (unsigned int)midImg,
                    mid + I(self, 0x78), I(self, 0x7c), patW, I(self, 0x88));
                mid = I(self, 0x94);
            }
            int rstart = I(self, 0xa0) + mid;
            if (rstart < filled) {
                int rImg = (C(self, 0xb8) == 0) ? I(self, 0x50) : I(self, 0x44);
                int rW = (filled - mid) - I(self, 0xa0);
                if (I(self, 0x9c) < rW) rW = I(self, 0x9c);
                PaintCanvas_DrawRegion2D(canvas, (unsigned int)rImg, 0, 0, rW,
                    I(self, 0x88), filled, 0, 0, 0, rstart + I(self, 0x78));
            }
            Layout_setDrawColor(layoutC, -1);
        }

        // label colour: tinted when disabled (0xb3).
        unsigned int lblColor = U(self, 0xac);
        if (C(self, 0xb3) != 0)
            PaintCanvas_SetColorARGB((unsigned char)(lblColor >> 24),
                (unsigned char)(lblColor >> 16), (unsigned char)(lblColor >> 8),
                (unsigned char)lblColor);
        else
            PaintCanvas_SetColor(0xffffffff);

        if (I(self, 0x24) == -1) {
            // primary label
            PaintCanvas_DrawString(canvas, (String *)((char *)self + 8), (char *)self + 0xc,
                I(self, 0x78) + I(self, 0xa4), (bool)(I(self, 0x7c) + I(self, 0xa8)));

            // secondary / value label (offset 0x18) when 0x20 set.
            if (I(self, 0x20) != 0) {
                String *t = (String *)((char *)self + 8);
                int px = I(self, 0x78);
                int w  = I(self, 0x90);
                int tx, ty;
                if (I(self, 0x70) == 10) {
                    int tw = PaintCanvas_GetTextWidth(canvas, t);
                    int th = PaintCanvas_GetTextHeight(canvas);
                    ty = I(self, 0x7c) + I(self, 0x88) + th * -2;
                    tx = (px + w / 2) - tw / 2;
                } else {
                    int off = I(self, 0xa4);
                    int tw  = PaintCanvas_GetTextWidth(canvas, t);
                    ty = I(self, 0x7c) + I(self, 0xa8);
                    tx = ((w + px) - off) - tw;
                }
                PaintCanvas_DrawString(canvas, t, (char *)self + 0x18, tx, (bool)ty);
            }

            // shortcut / corner label (offset 0x2c) when 0x34 set.
            if (I(self, 0x34) != 0) {
                PaintCanvas_SetColor(0xffffffff);
                String *u = (String *)(*g_TB_d_unitStr);
                int tw = PaintCanvas_GetTextWidth(canvas, u);
                PaintCanvas_DrawString(canvas, u, (char *)self + 0x2c,
                    (I(self, 0x94) + I(self, 0x78)) - tw, (bool)(I(self, 0x7c) + I(self, 0xa8)));
                PaintCanvas_SetColor(0xffffffff);
            }

            // small adornment image (offset 0x38) when set.
            if (I(self, 0x38) != -1) {
                PaintCanvas_SetColor(0xffffffff);
                PaintCanvas_DrawImage2DEx(canvas, I(self, 0x38),
                    (I(self, 0x78) + I(self, 0x90) + 6) - I(self, 0x94),
                    I(self, 0x7c) + 1, 0x11, 0x14);
                PaintCanvas_SetColor(0xffffffff);
            }
        } else if (I(self, 0x70) != 0x13) {
            icon = base;
            iconY = I(self, 0xa4) + I(self, 0x78);
            tailIcon = true;
        }
    }

    if (tailIcon)
        PaintCanvas_DrawImage2D(canvas, icon, I(self, 0x78) + I(self, 0xa4) + 0, iconY);

    Layout_setDrawColor(*g_TB_d_layoutEnd, -1);
    PaintCanvas_FontSetSpacing(canvas, U(self, 0x8), savedSpacing);
    PaintCanvas_SetColor(savedColor);
}

// ---- TouchButton_168e88.cpp ----
// TouchButton::TouchButton(String const& text, int type, int x, int y, int p5, uchar p6, uchar p7)
// Default-constructs the three embedded label Strings (+0xc/+0x18/+0x2c), seeds the font color
// (+0x08) from the active-font global, caches the current font spacing (+0xc4), then delegates
// to init() which builds the button's images and layout.

extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor7(TouchButton *self, String *text, int type, int x, int y,
                                          int p5, unsigned char p6, unsigned char p7)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, (unsigned int)type, x, y, p5, 0, 0, 0, p6, p7);
    return self;
}

// ---- setText_1691b0.cpp ----
extern "C" void String_assign(void *dst, String *src);                   // String::operator=
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TB_canvas3;

extern "C" void TouchButton_setText(TouchButton *self, String *text)
{
    String_assign((char *)self + 0xc, text);
    void **holder = g_TB_canvas3;
    int w = PaintCanvas_GetTextWidth2(*holder, P(self, 8), text);
    if (I(self, 0x24) != -1)
        w = PaintCanvas_GetImage2DWidth(*holder);
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
    int th = PaintCanvas_GetTextHeight(*holder);
    th = (h - th) / 2;
    I(self, 0xa8) = th;
    if (I(self, 0x70) == 3)
        I(self, 0xa8) = th + 2;
    if (I(self, 0x24) != -1) {
        h = I(self, 0x88);
        int ih = PaintCanvas_GetImage2DHeight(*holder);
        I(self, 0xa8) = (h - ih) / 2;
        if (I(self, 0x70) == 1)
            I(self, 0xa4) = I(self, 0xa4) + 3;
    }
    return TouchButton_setPosition(self, I(self, 0x80), I(self, 0x84), UC(self, 0x74));
}

// ---- setYPosition_169374.cpp ----
extern "C" void TouchButton_setYPosition(TouchButton *self, int y)
{
    return TouchButton_setPosition(self, I(self, 0x78), y, UC(self, 0x74));
}

// ---- touchedInside_169938.cpp ----
// Singleton pointer dereferenced twice (PaintCanvas-like). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern int **g_TB_canvas;

extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py)
{
    int x = I(self, 0x78);
    int xm1 = x - 1;
    int h;
    int top;
    if (I(self, 0x70) == 3) {
        int v = (*g_TB_canvas)[0x38 / 4];
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

extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing;
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];

extern "C" void TouchButton_168d9c(TouchButton *self, unsigned int kind, unsigned int image,
                                   int a, int b, int c, unsigned char flag)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x28) = image;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(canvas);

    unsigned char tmp[12];       // String is a 12-byte value type
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    TouchButton_init(self, (String *)tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
    String_dtor(tmp);
}

// ---- TouchButton_1684f0.cpp ----
// TouchButton::TouchButton(String const& text, int x, int y, int p4, uchar p5)
// 5-arg overload: same setup as the other label ctors -- three embedded Strings, font color
// (+0x08), cached spacing (+0xc4) -- then init().

extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor5(TouchButton *self, String *text, int x, int y,
                                          int p4, unsigned char p5)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, 0xffffffff, 4, x, y, p4, 0, 0, p5, 0x44);
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
extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

// PaintCanvas singleton (deref twice). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;

extern "C" TouchButton *TouchButton_168f30(TouchButton *self, String *text,
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
    short prev = PaintCanvas_FontGetSpacing(canvas);
    PaintCanvas_FontSetSpacing(canvas, spacing, (short)kerning);

    TouchButton_init(self, text, spacing, a, b, c, d, -1, -1, flags0, flags1);

    PaintCanvas_FontSetSpacing(canvas, spacing, prev);
    return self;
}

// ---- TouchButton_1690e4.cpp ----
extern "C" void String_ctor(void *s);   // AbyssEngine::String::String()
extern "C" void TouchButton_init(TouchButton *self, String *text, unsigned int a,
                                 int b, int x, int y, int p4, int p5, int z,
                                 unsigned char p6, unsigned char c);
// Two singletons (cell value = holder; one deref yields value/object).
__attribute__((visibility("hidden"))) extern int *g_TB_c1;
__attribute__((visibility("hidden"))) extern void **g_TB_c2;

extern "C" TouchButton *TouchButton_ctor6(TouchButton *self, int x, int y, String *text,
                                          int p4, int p5, unsigned char p6)
{
    String_ctor((char *)self + 0xc);
    String_ctor((char *)self + 0x18);
    String_ctor((char *)self + 0x2c);
    I(self, 8) = *(int *)*g_TB_c1;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(*g_TB_c2);
    TouchButton_init(self, text, 0xffffffff, 4, x, y, p4, p5, 0, p6, 0x44);
    return self;
}

// ---- TouchButton_168cb0.cpp ----
// TouchButton::TouchButton(unsigned int, int, int, int, unsigned char)
//
// Five-argument convenience constructor: kind + position + a single flag byte.
// Builds the three embedded Strings, samples the default spacing/kerning, then
// runs init() with an empty label and a fixed extra parameter (0x44).

extern "C" void  TouchButton_init(TouchButton *self, String *text,
                                  unsigned int kind, int a, int b, int c, int d,
                                  int x, int y,
                                  unsigned char flags0, unsigned char flags1);

__attribute__((visibility("hidden"))) extern void **g_TB_canvas_ctor;
__attribute__((visibility("hidden"))) extern unsigned int *g_TB_defSpacing;
__attribute__((visibility("hidden"))) extern const char  g_TB_emptyStr[];

extern "C" void TouchButton_168cb0(TouchButton *self, unsigned int kind,
                                   int a, int b, int c, unsigned char flag)
{
    String_ctor_default((char *)self + 0xc);
    String_ctor_default((char *)self + 0x18);
    String_ctor_default((char *)self + 0x2c);

    void *canvas = *g_TB_canvas_ctor;
    U(self, 0x8)  = *g_TB_defSpacing;
    I(self, 0xc4) = PaintCanvas_FontGetSpacing(canvas);

    unsigned char tmp[12];       // String is a 12-byte value type
    String_ctor_cstr(tmp, g_TB_emptyStr, false);
    TouchButton_init(self, (String *)tmp, kind, a, b, c, 0x44, -1, -1, flag, 0);
    String_dtor(tmp);
}
