#include "gof2/engine/render/ImageFactory.h"
#include "gof2/engine/render/ImagePart.h"
#include "gof2/engine/render/PaintCanvas.h"

extern "C" void *Sprite_dtor(void *p);
namespace AbyssEngine { namespace AERandom { int nextInt(void *random, int limit); } }
extern "C" void Sprite_ctor(void *self, void *ids, int n, int w, int h);
extern "C" __attribute__((visibility("hidden"))) GetTextFn *g_reload_getText;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_reload_canvas;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawChar_canvas;
extern "C" void IF_Sprite_setFrame(void *spr, int frame);
extern "C" long long IF_Sprite_setPosition(void *spr, int x, int y);
extern "C" void IF_Sprite_draw(float sx, float sy);
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawShip_canvas;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_drawItem_canvas;
extern "C" void IF_ImagePart_ctor(void *self, unsigned id, int x, int y);
extern "C" __attribute__((visibility("hidden"))) int *g_IF_idTable;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_IF_drawItem4_canvas;
int *ImageFactory_createChar_bi(int param_1, int param_2, int sel);
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagA;
extern "C" __attribute__((visibility("hidden"))) char *g_ctor_flagB;
extern "C" __attribute__((visibility("hidden"))) int *g_ctor_dst;

// ImageFactory::getItemImageId(int) -> base id (0x898 below 0xb0, else 0xef0) + param.
int ImageFactory::getItemImageId(int param_1)
{
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    return base + param_1;
}

// ImageFactory::~ImageFactory() -> returns this. Frees the owned Sprite at +0x00.
ImageFactory *_ZN12ImageFactoryD2Ev(ImageFactory *self)
{
    void *p = self->sprite;
    if (p != 0) ::operator delete(Sprite_dtor(p));
    self->sprite = 0;
    return self;
}

// ImageFactory::~ImageFactory() — destroys the owned Sprite stored at +0x00 and
// clears the slot, matching the binary destructor (Sprite::~Sprite + operator delete).
ImageFactory::~ImageFactory()
{
    void *p = this->sprite;
    if (p != 0) {
        ::operator delete(Sprite_dtor(p));
    }
    this->sprite = 0;
}

// *gCreateChar2Rng1 -> rng (used for the type-3 reroll); gCreateChar2Table -> int[] table base
// (PC-relative address, not dereferenced); *gCreateChar2Rng2 -> rng for the per-part loop.
extern void *const gCreateChar2Rng1 __attribute__((visibility("hidden")));
extern int gCreateChar2Table __attribute__((visibility("hidden")));
extern void *const gCreateChar2Rng2 __attribute__((visibility("hidden")));

// ImageFactory::createChar(bool, int) — builds a 5-int char descriptor (type + 4 random parts).
// r0=param_1(bool, unused on this path), r1=param_2, r2=sel.
int *ImageFactory_createChar_bi(int param_1, int param_2, int sel)
{
    (void)param_1;
    if (sel == 3) {
        int t = AbyssEngine::AERandom::nextInt(*(void **)gCreateChar2Rng1, 4);
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
        obj[i + 1] = AbyssEngine::AERandom::nextInt(*(void **)gCreateChar2Rng2, base[i]);
    return obj;
}

// Tail veneer: another Image2DCreate-like through a function-pointer global.
// *g_reload_getText -> getText-style fn ptr; *g_reload_canvas -> canvas holder.

// ImageFactory::reload() -- rebuilds the character image-id table + composite sprite.
void ImageFactory::reload() {
    unsigned *ids = (unsigned *)operator new[](0x18);
    unsigned *holder = g_reload_canvas;
    GetTextFn getText = *g_reload_getText;
    getText(*holder, 0x4fa, ids);
    getText(*holder, 0x4fb, ids + 1);
    getText(*holder, 0x4f7, ids + 2);
    getText(*holder, 0x4f8, ids + 3);
    getText(*holder, 0x4f9, ids + 4);
    getText(*holder, 0x4fc, ids + 5);
    void *old = this->sprite;
    if (old != 0) ::operator delete(Sprite_dtor(old));
    this->sprite = 0;
    void *spr = operator new(0x40);
    int w = ((PaintCanvas*)(long)*holder)->GetImage2DWidth((int)ids[0]);
    int h = ((PaintCanvas*)(long)*holder)->GetImage2DHeight((int)ids[0]);
    Sprite_ctor(spr, ids, 6, w, h);
    this->sprite = spr;
    ((PaintCanvas*)(long)*holder)->Image2DCreate(0x485, &this->itemImage);
    return ImageFactory::reload_tail(*holder, 0x511, &this->shipImage);
}

// Tail veneer (function-pointer global): draws the foreground glyph layer.

// ImageFactory::drawChar(Array<ImagePart*>*, int, int, bool)
void ImageFactory::drawChar(Array<ImagePart *> *parts, int x, int y, int flag) {
    unsigned *holder = g_drawChar_canvas;
    ((PaintCanvas*)(long)*holder)->SetColor(0xffffffffu);
    ((PaintCanvas*)(long)*holder)->DrawImage2D(this->itemImage, x, y);
    for (unsigned i = 0; i < parts->size(); ++i) {
        ImagePart *part = (*parts)[i];
        if (part != 0) part->draw(x, y, flag);
    }
    return ImageFactory::drawChar_tail(*holder, this->shipImage, x, y);
}

// ImageFactory::drawShip(int shipId, int x, int y) — draws the composite ship sprite (frame 5)
// at (x,y), then overlays the ship's class icon (image id 0x971+shipId).

void ImageFactory::drawShip(int shipId, int x, int y) {
    unsigned *holder = g_IF_drawShip_canvas;
    unsigned local = 0xffffffffu;
    ((PaintCanvas*)(long)*holder)->SetColor(0xffffffffu);
    IF_Sprite_setFrame(this->sprite, 5);
    IF_Sprite_setPosition(this->sprite, x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    ((PaintCanvas*)(long)*holder)->Image2DCreate((unsigned short)(shipId + 0x971), &local);
    ((PaintCanvas*)(long)*holder)->DrawImage2D((int)local, x, y);
}

// canvas holder: *g_holder yields the canvas handle (matches the target's double-load).

// ImageFactory::drawItem(int, int, int) — draws the item icon for id param_1 at (param_2,param_3).
void ImageFactory_drawItem3(int param_1, int param_2, int param_3)
{
    unsigned *holder = g_drawItem_canvas;
    unsigned local;
    ((PaintCanvas*)(long)*holder)->SetColor(0xffffffffu);
    local = 0xffffffffu;
    int base = 0xef0;
    if (param_1 < 0xb0) base = 0x898;
    ((PaintCanvas*)(long)*holder)->Image2DCreate((unsigned short)(base + param_1), &local);
    ((PaintCanvas*)(long)*holder)->DrawImage2D(local, param_2, param_3);
}

// ImageFactory::drawItem(int itemId, int x, int y) — draws just the item icon (no composite
// sprite) for itemId at (x,y): id base is 0x898 for ids < 0xb0, otherwise 0xef0.
void ImageFactory::drawItem(int itemId, int x, int y)
{
    unsigned *holder = g_drawItem_canvas;
    unsigned local = 0xffffffffu;
    ((PaintCanvas*)(long)*holder)->SetColor(0xffffffffu);
    int base = 0xef0;
    if (itemId < 0xb0)
        base = 0x898;
    ((PaintCanvas*)(long)*holder)->Image2DCreate((unsigned short)(base + itemId), &local);
    ((PaintCanvas*)(long)*holder)->DrawImage2D(local, x, y);
}

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

void * ImageFactory::loadImage(int row, int col, int frameBase) {
    int id = g_IF_idTable[row * 4 + col];   // 0x10-byte rows / 4-byte cells
    if (id < 0)
        return 0;

    unsigned local = 0;
    ((PaintCanvas*)(long)*g_IF_li_canvas)->Image2DCreate((unsigned short)((short)id + (short)frameBase),
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
    return part;
}

// ImageFactory::drawItem(int itemId, int frame, int x, int y) — draws the item's composite
// sprite at the given frame/position, then overlays the item icon (0x898 for ids < 0xb0,
// otherwise 0xef0, plus itemId).

void ImageFactory::drawItem4(int itemId, int frame, int x, int y) {
    unsigned *holder = g_IF_drawItem4_canvas;
    unsigned local = 0xffffffffu;
    ((PaintCanvas*)(long)*holder)->SetColor(0xffffffffu);
    IF_Sprite_setFrame(this->sprite, frame);
    IF_Sprite_setPosition(this->sprite, x, y);
    IF_Sprite_draw(1.0f, 1.0f);
    int base = 0xef0;
    if (itemId < 0xb0)
        base = 0x898;
    ((PaintCanvas*)(long)*holder)->Image2DCreate((unsigned short)(base + itemId), &local);
    ((PaintCanvas*)(long)*holder)->DrawImage2D((int)local, x, y);
}

// ImageFactory::loadChar(int*) -> Array<ImagePart*>* of 4 entries (with [0]/[2] swapped).
Array<ImagePart *> * ImageFactory::loadChar(int *param_1) {
    (void)this;
    if (param_1 == 0) return 0;
    Array<ImagePart *> *a = new Array<ImagePart *>();
    a->resize(4);
    int first = *param_1++;
    for (int i = 0; i != 4; ++i) {
        int raw = param_1[i];
        void *part = (void *)(raw + 1);
        if (part != 0)
            (*a)[i] = (ImagePart *)((ImageFactory *)part)->loadImage(first, i, raw);
    }
    ImagePart *tmp = (*a)[0];
    (*a)[0] = (*a)[2];
    (*a)[2] = tmp;
    return a;
}

// createChar(bool,int) — 3 register args in practice (r0=clz bool, r1=rand==0, r2=type).
// PC-relative pointer-to-pointer global: *g -> rng object.
extern void *const gCreateCharRng __attribute__((visibility("hidden")));

// ImageFactory::createChar(int) — randomizes a char then forwards to createChar(bool,int).
void ImageFactory::createChar_i(int param_1) {
    (void)this;
    int r = AbyssEngine::AERandom::nextInt(*(void **)gCreateCharRng, 2);
    ImageFactory_createChar_bi((int)__builtin_clz(r), (unsigned)(r == 0), param_1);
}

// ImageFactory::createChar(bool, int) — builds a 5-int char descriptor: a type slot followed by
// four random part indices. `type` selects a 4-entry row in the part-count table (rows are 16
// bytes); type 3 rerolls to 0 or 2; type 0 maps to row 10 unless rand0 forced it; type 5 -> 0.
int *ImageFactory::createChar(bool clz, bool rand0, int type)
{
    (void)clz;
    if (type == 3) {
        int t = AbyssEngine::AERandom::nextInt(*(void **)gCreateChar2Rng1, 4);
        type = (t != 0) ? 2 : 0;
    }
    int v = type;
    int *table = &gCreateChar2Table;
    if (type == 0) v = 10;
    if (rand0) v = type;
    if (v == 5) v = 0;
    int *obj = (int *)operator new[](0x14);
    obj[0] = v;
    int *base = (int *)((char *)table + v * 16);
    for (int i = 0; i != 4; ++i)
        obj[i + 1] = AbyssEngine::AERandom::nextInt(*(void **)gCreateChar2Rng2, base[i]);
    return obj;
}

// *g_ctor_flagA / *g_ctor_flagB -> gate bytes (both evaluated). *g_ctor_dst -> int* dest table;
// g_ctor_src -> source int table (PC-relative base, not dereferenced).
extern int g_ctor_src[] __attribute__((visibility("hidden")));

// ImageFactory::ImageFactory() -> this. Copies a 13x4x2 int table when gated, then reload().
ImageFactory::ImageFactory() {
    this->sprite = 0;
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
    this->reload();
}

// Tail-call fragment at the end of reload(): create image id 0x511 into the
// destination pointer (self+8). The decompiler routed this through a GOT code
// pointer; it is PaintCanvas::Image2DCreate(canvas, id, out).
void ImageFactory::reload_tail(unsigned canvas, int id, void *out) {
    ((PaintCanvas *)(long)canvas)->Image2DCreate((unsigned short)id, (unsigned int *)out);
}

// Tail-call fragment at the end of drawChar(): draw the foreground glyph layer
// (image2d handle stored at self+8) at (x,y). It is PaintCanvas::DrawImage2D.
void ImageFactory::drawChar_tail(unsigned canvas, int handle, int x, int y) {
    ((PaintCanvas *)(long)canvas)->DrawImage2D((unsigned int)handle, x, y);
}
