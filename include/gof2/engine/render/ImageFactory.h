#ifndef GOF2_IMAGEFACTORY_H
#define GOF2_IMAGEFACTORY_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ImageFactory (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "ImageFactory::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Known layout (from ctor/reload/draw decompiles):
//   +0x00  Sprite*  itemSprite (operator new(0x40) Sprite)
//   +0x04  Array<ImagePart*>*  (item image2d handle, used by drawChar via this+4)
//   +0x08  int      ship image2d handle

void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

struct ImageFactory;
class ImagePart;

// Field accessor via byte offset.

// reload() pulls glyph image ids through a function-pointer global (a getText-style fetch).
typedef void (*GetTextFn)(unsigned canvas, int id, void *out);

// Plain view over the engine's Array<ImagePart*> (count / data ptr / capacity) used by the
// drawChar / loadChar paths.
struct Arr { uint32_t size; void **data; uint32_t size2; };

class ImageFactory {
public:
    void* sprite;                    // +0x0

    ~ImageFactory();

    // ---- methods (converted from free functions) ----
    void createChar_i(int param_1);
    // The demangled symbol reads ImageFactory::createChar(bool, int); in practice the target
    // passes three register args (r0=clz bool, r1=rand==0, r2=type), so the faithful body needs
    // all three. createChar_i() forwards (clz, rand0, type) here.
    int *createChar(bool clz, bool rand0, int type);
    // Public ImageFactory::createChar(bool, int) overload (2 register args at the call site);
    // forwards to the faithful 3-arg builder with rand0=false.
    int *createChar(bool clz, int type) { return createChar(clz, false, type); }
    ImageFactory * ctor();
    void drawChar(Array<ImagePart *> *parts, int x, int y, int flag);
    void drawItem(int itemId, int x, int y);
    void drawItem4(int itemId, int frame, int x, int y);
    void drawShip(int shipId, int x, int y);
    Array<ImagePart *> * loadChar(int *param_1);
    void * loadImage(int row, int col, int frameBase);
    void reload();

    // Tail-call fragments emitted at the end of reload()/drawChar(): both simply
    // forward to a PaintCanvas method on the supplied canvas handle.
    //   reload_tail  -> PaintCanvas::Image2DCreate(canvas, id, out)
    //   drawChar_tail-> PaintCanvas::DrawImage2D(canvas, handle, x, y)
    static void reload_tail(unsigned canvas, int id, void *out);
    static void drawChar_tail(unsigned canvas, int handle, int x, int y);

    int getItemImageId(int param_1);
};
#endif
