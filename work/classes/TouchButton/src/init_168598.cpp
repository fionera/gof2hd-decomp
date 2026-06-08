#include "class.h"

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

extern "C" void  String_ctor_cstr(void *s, const char *text, bool copy);
extern "C" void  String_dtor(void *s);
extern "C" void  String_assign(void *dst, String *src);                 // String::operator=
extern "C" void  String_assign_cstr_tmp(void *dst, const char *lit);    // assign from literal via temp

extern "C" int   Achievements_isEliteMedal(void *ach, int id);
extern "C" void  PaintCanvas_Image2DCreate(void *canvas, unsigned short imgId, unsigned int *outHandle);
extern "C" int   PaintCanvas_GetImage2DWidth(void *canvas);
extern "C" int   PaintCanvas_GetImage2DHeight(void *canvas);
extern "C" int   PaintCanvas_GetTextWidth(void *canvas, String *text);
extern "C" int   PaintCanvas_GetTextHeight(void *canvas);
extern "C" unsigned int GameText_getLanguage();
extern "C" void  TouchButton_setText(TouchButton *self, String *text);
extern "C" void  TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);

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
