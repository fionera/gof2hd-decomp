#include "gof2/ImageFactory.h"


extern "C" void *Sprite_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" int AERandom_nextInt(int rng, int bound);
extern "C" int PaintCanvas_GetImage2DWidth(unsigned canvas, int id);
extern "C" int PaintCanvas_GetImage2DHeight(unsigned canvas, int id);
extern "C" void Sprite_ctor(void *self, void *ids, int n, int w, int h);
extern "C" void PaintCanvas_Image2DCreate(unsigned canvas, int id, void *out);
extern "C" void ImageFactory_reload_tail(unsigned canvas, int id, void *out);
extern "C" __attribute__((visibility("hidden"))) GetTextFn *g_reload_getText;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_reload_canvas;
extern "C" void PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
extern "C" void ImagePart_draw(void *part, int x, int y, int flag);
extern "C" void ImageFactory_drawChar_tail(unsigned canvas, int handle, int x, int y);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawChar_canvas;
extern "C" void IF_Sprite_setFrame(void *spr, int frame);
extern "C" long long IF_Sprite_setPosition(void *spr, int x, int y);
extern "C" void IF_Sprite_draw(float sx, float sy);
extern "C" void IF_PaintCanvas_SetColor(unsigned canvas, unsigned color);
extern "C" void IF_PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" void IF_PaintCanvas_DrawImage2D(unsigned canvas, int image, int x, int y);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawShip_canvas;
extern "C" void PaintCanvas_Image2DCreate(unsigned canvas, unsigned short id, unsigned *out);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawItem_canvas;
extern "C" void IF_ImagePart_ctor(void *self, unsigned id, int x, int y);
extern "C" __attribute__((visibility("hidden"))) int *g_IF_idTable;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawItem4_canvas;
extern "C" void Array_ImagePart_ctor(Arr *a);
extern "C" void ArraySetLength_ImagePart(uint32_t n, Arr *a);
extern "C" void *ImageFactory_loadImage(void *thisish, int a, int b, int c);
extern "C" void ImageFactory_createChar_bi(int a, int b, int type);
extern "C" void ImageFactory_reload(ImageFactory *self);
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagA;
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagB;
extern "C" __attribute__((visibility("hidden"))) int *g_ctor_dst;

// ---- getItemImageId_11ca50.cpp ----
// ImageFactory::getItemImageId(int) -> base id (0x898 below 0xb0, else 0xef0) + param.
struct ImageFactory { int getItemImageId(int param_1); };
int ImageFactory::getItemImageId(int param_1)
{
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    return base + param_1;
}

// ---- _ImageFactory_11c6b0.cpp ----
// ImageFactory::~ImageFactory() -> returns this. Frees the owned Sprite at +0x00.
extern "C" ImageFactory *_ZN12ImageFactoryD2Ev(ImageFactory *self)
{
    void *p = self->field_0x0;
    if (p != 0) operator_delete(Sprite_dtor(p));
    self->field_0x0 = 0;
    return self;
}

// ---- createChar_11c6f4.cpp ----
// *gCreateChar2Rng1 -> rng (used for the type-3 reroll); gCreateChar2Table -> int[] table base
// (PC-relative address, not dereferenced); *gCreateChar2Rng2 -> rng for the per-part loop.
extern void *const gCreateChar2Rng1 __attribute__((visibility("hidden")));
extern int gCreateChar2Table __attribute__((visibility("hidden")));
extern void *const gCreateChar2Rng2 __attribute__((visibility("hidden")));

// ImageFactory::createChar(bool, int) — builds a 5-int char descriptor (type + 4 random parts).
// r0=param_1(bool, unused on this path), r1=param_2, r2=sel.
extern "C" int *ImageFactory_createChar_bi(int param_1, int param_2, int sel)
{
    (void)param_1;
    if (sel == 3) {
        int t = AERandom_nextInt(*(int *)gCreateChar2Rng1, 4);
        sel = (t != 0) ? 2 : 0;
    }
    int v = sel;
    int *table = &gCreateChar2Table;
    if (sel == 0) v = 10;
    if (param_2 != 0) v = sel;
    if (v == 5) v = 0;
    int *obj = (int *)operator new[](0x14);
    obj[0] = v;
    int *base = (int *)((char *)table + v * 16);
    for (int i = 0; i != 4; ++i)
        obj[i + 1] = AERandom_nextInt(*(int *)gCreateChar2Rng2, base[i]);
    return obj;
}

// ---- reload_11c5d0.cpp ----
typedef void (*GetTextFn)(unsigned canvas, int id, void *out);
// Tail veneer: another Image2DCreate-like through a function-pointer global.
// *g_reload_getText -> getText-style fn ptr; *g_reload_canvas -> canvas holder.

// ImageFactory::reload() -- rebuilds the character image-id table + composite sprite.
extern "C" void ImageFactory_reload(ImageFactory *self)
{
    unsigned *ids = (unsigned *)operator new[](0x18);
    unsigned *holder = g_reload_canvas;
    GetTextFn getText = *g_reload_getText;
    getText(*holder, 0x4fa, ids);
    getText(*holder, 0x4fb, ids + 1);
    getText(*holder, 0x4f7, ids + 2);
    getText(*holder, 0x4f8, ids + 3);
    getText(*holder, 0x4f9, ids + 4);
    getText(*holder, 0x4fc, ids + 5);
    void *old = self->field_0x0;
    if (old != 0) operator_delete(Sprite_dtor(old));
    self->field_0x0 = 0;
    void *spr = operator new(0x40);
    int w = PaintCanvas_GetImage2DWidth(*holder, (int)ids[0]);
    int h = PaintCanvas_GetImage2DHeight(*holder, (int)ids[0]);
    Sprite_ctor(spr, ids, 6, w, h);
    self->field_0x0 = spr;
    PaintCanvas_Image2DCreate(*holder, 0x485, (char *)self + 4);
    return ImageFactory_reload_tail(*holder, 0x511, (char *)self + 8);
}

// ---- drawChar_11c940.cpp ----
struct Arr { uint32_t size; void **data; uint32_t size2; };

// Tail veneer (function-pointer global): draws the foreground glyph layer.

// ImageFactory::drawChar(Array<ImagePart*>*, int, int, bool)
extern "C" void ImageFactory_drawChar(ImageFactory *self, Arr *parts, int x, int y, int flag)
{
    unsigned *holder = g_drawChar_canvas;
    PaintCanvas_SetColor(*holder, 0xffffffffu);
    PaintCanvas_DrawImage2D(*holder, i32(self, 0x4), x, y);
    for (unsigned i = 0; i < parts->size; ++i) {
        void *part = parts->data[i];
        if (part != 0) ImagePart_draw(part, x, y, flag);
    }
    return ImageFactory_drawChar_tail(*holder, i32(self, 0x8), x, y);
}

// ---- drawShip_11cad0.cpp ----
// ImageFactory::drawShip(int shipId, int x, int y) — draws the composite ship sprite (frame 5)
// at (x,y), then overlays the ship's class icon (image id 0x971+shipId).

extern "C" void ImageFactory_drawShip(ImageFactory *self, int shipId, int x, int y)
{
    unsigned *holder = g_IF_drawShip_canvas;
    unsigned local = 0xffffffffu;
    IF_PaintCanvas_SetColor(*holder, 0xffffffffu);
    IF_Sprite_setFrame(self->field_0x0, 5);
    IF_Sprite_setPosition(self->field_0x0, x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    IF_PaintCanvas_Image2DCreate(*holder, (unsigned short)(shipId + 0x971), &local);
    IF_PaintCanvas_DrawImage2D(*holder, (int)local, x, y);
}

// ---- drawItem_11ca60.cpp ----
// canvas holder: *g_holder yields the canvas handle (matches the target's double-load).

// ImageFactory::drawItem(int, int, int) — draws the item icon for id param_1 at (param_2,param_3).
extern "C" void ImageFactory_drawItem3(int param_1, int param_2, int param_3)
{
    unsigned *holder = g_drawItem_canvas;
    unsigned local;
    PaintCanvas_SetColor(*holder, 0xffffffffu);
    local = 0xffffffffu;
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    PaintCanvas_Image2DCreate(*holder, (unsigned short)(base + param_1), &local);
    PaintCanvas_DrawImage2D(*holder, local, param_2, param_3);
}

// ---- loadImage_11c834.cpp ----
// ImageFactory::loadImage(int row, int col, int frameBase) — looks up the image id for the
// [row][col] cell, creates the image2d in the global canvas, then builds an ImagePart whose
// position is taken from one of several layout tables selected by two orientation flags.

// id table: base[row*0x10 + col*4] -> image id (negative means "no image")
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_li_canvas; // canvas holder
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagA;        // first orientation flag
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagB;        // second orientation flag
extern "C" __attribute__((visibility("hidden"))) int g_IF_posTableA[];    // when flagA set
extern "C" __attribute__((visibility("hidden"))) int g_IF_posTableB[];    // when flagB set
extern "C" __attribute__((visibility("hidden"))) int *g_IF_posTableC;     // default base ptr
extern "C" __attribute__((visibility("hidden"))) char *g_IF_flagC;        // selects C vs D
extern "C" __attribute__((visibility("hidden"))) int *g_IF_posTableD;     // alt default base ptr

extern "C" void ImageFactory_loadImage(ImageFactory *self, int row, int col, int frameBase)
{
    int id = g_IF_idTable[row * 4 + col];   // 0x10-byte rows / 4-byte cells
    if (id < 0)
        return;

    unsigned local = 0;
    IF_PaintCanvas_Image2DCreate(*g_IF_li_canvas, (unsigned short)((short)id + (short)frameBase),
                                 &local);
    void *part = operator new(0x14);

    int *posBase;
    int rowCol = row * 0x20 + col * 8;
    if (*g_IF_flagA != 0) {
        posBase = g_IF_posTableA;       // base + rowCol applied below
    } else if (*g_IF_flagB != 0) {
        posBase = g_IF_posTableB;
    } else {
        posBase = g_IF_posTableC;
        if (*g_IF_flagC == 0)
            posBase = g_IF_posTableD;
    }

    int x = *(int *)((char *)posBase + rowCol);
    int y = *(int *)((char *)posBase + rowCol + 4);
    IF_ImagePart_ctor(part, local, x, y);
}

// ---- drawItem_11c9b4.cpp ----
// ImageFactory::drawItem(int itemId, int frame, int x, int y) — draws the item's composite
// sprite at the given frame/position, then overlays the item icon (0x898 for ids < 0xb0,
// otherwise 0xef0, plus itemId).

extern "C" void ImageFactory_drawItem4(ImageFactory *self, int itemId, int frame, int x, int y)
{
    unsigned *holder = g_IF_drawItem4_canvas;
    unsigned local = 0xffffffffu;
    IF_PaintCanvas_SetColor(*holder, 0xffffffffu);
    IF_Sprite_setFrame(self->field_0x0, frame);
    IF_Sprite_setPosition(self->field_0x0, x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    int base = 0xef0;
    if (itemId < 0xb0)
        base = 0x898;
    IF_PaintCanvas_Image2DCreate(*holder, (unsigned short)(base + itemId), &local);
    IF_PaintCanvas_DrawImage2D(*holder, (int)local, x, y);
}

// ---- loadChar_11c774.cpp ----
struct Arr { uint32_t size; void **data; uint32_t size2; };


// ImageFactory::loadChar(int*) -> Array<ImagePart*>* of 4 entries (with [0]/[2] swapped).
extern "C" Arr *ImageFactory_loadChar(ImageFactory *self, int *param_1)
{
    (void)self;
    if (param_1 == 0) return 0;
    Arr *a = (Arr *)operator new(0xc);
    Array_ImagePart_ctor(a);
    ArraySetLength_ImagePart(4, a);
    int first = *param_1++;
    for (int i = 0; i != 4; ++i) {
        int raw = param_1[i];
        void *part = (void *)(raw + 1);
        if (part != 0)
            a->data[i] = ImageFactory_loadImage(part, first, i, raw);
    }
    void *tmp = a->data[0];
    a->data[0] = a->data[2];
    a->data[2] = tmp;
    return a;
}

// ---- createChar_11c6cc.cpp ----
// createChar(bool,int) — 3 register args in practice (r0=clz bool, r1=rand==0, r2=type).
// PC-relative pointer-to-pointer global: *g -> rng object.
extern void *const gCreateCharRng __attribute__((visibility("hidden")));

// ImageFactory::createChar(int) — randomizes a char then forwards to createChar(bool,int).
extern "C" void ImageFactory_createChar_i(ImageFactory *self, int param_1)
{
    (void)self;
    int r = AERandom_nextInt(*(int *)gCreateCharRng, 2);
    ImageFactory_createChar_bi((int)__builtin_clz(r), (unsigned)(r == 0), param_1);
}

// ---- ImageFactory_11c54c.cpp ----
// *g_ctor_flagA / *g_ctor_flagB -> gate bytes (both evaluated). *g_ctor_dst -> int* dest table;
// g_ctor_src -> source int table (PC-relative base, not dereferenced).
extern int g_ctor_src[] __attribute__((visibility("hidden")));

// ImageFactory::ImageFactory() -> this. Copies a 13x4x2 int table when gated, then reload().
extern "C" ImageFactory *ImageFactory_ctor(ImageFactory *self)
{
    self->field_0x0 = 0;
    if ((*g_ctor_flagA | *g_ctor_flagB) != 0) {
        int *dst = g_ctor_dst;
        int *src = g_ctor_src;
        for (int r = 0; r != 0xd; ++r) {
            int *d = dst;
            int *s = src;
            for (int c = 0; c != 4; ++c) {
                for (int k = 0; k != 2; ++k)
                    d[k] = s[k];
                s += 2;
                d += 2;
            }
            dst += 8;
            src += 8;
        }
    }
    ImageFactory_reload(self);
    return self;
}
