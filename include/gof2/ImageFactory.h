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

// Field accessor via byte offset.


static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

// reload() pulls glyph image ids through a function-pointer global (a getText-style fetch).
typedef void (*GetTextFn)(unsigned canvas, int id, void *out);

// Plain view over the engine's Array<ImagePart*> (count / data ptr / capacity) used by the
// drawChar / loadChar paths.
struct Arr { uint32_t size; void **data; uint32_t size2; };

struct ImageFactory {
    void* field_0x0;                    // +0x0

    // ---- methods (converted from free functions) ----
    void createChar_i(int param_1);
    ImageFactory * ctor();
    void drawChar(Arr *parts, int x, int y, int flag);
    void drawItem4(int itemId, int frame, int x, int y);
    void drawShip(int shipId, int x, int y);
    Arr * loadChar(int *param_1);
    void * loadImage(int row, int col, int frameBase);
    void reload();

    int getItemImageId(int param_1);
};
#endif
