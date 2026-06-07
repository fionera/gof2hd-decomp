#include "class.h"

typedef void (*GetTextFn)(unsigned canvas, int id, void *out);
extern "C" void *Sprite_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" int PaintCanvas_GetImage2DWidth(unsigned canvas, int id);
extern "C" int PaintCanvas_GetImage2DHeight(unsigned canvas, int id);
extern "C" void Sprite_ctor(void *self, void *ids, int n, int w, int h);
extern "C" void PaintCanvas_Image2DCreate(unsigned canvas, int id, void *out);
// Tail veneer: another Image2DCreate-like through a function-pointer global.
extern "C" void ImageFactory_reload_tail(unsigned canvas, int id, void *out);
// *g_reload_getText -> getText-style fn ptr; *g_reload_canvas -> canvas holder.
extern "C" __attribute__((visibility("hidden"))) GetTextFn *g_reload_getText;
extern "C" __attribute__((visibility("hidden"))) unsigned *g_reload_canvas;

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
    void *old = F<void *>(self, 0x0);
    if (old != 0) operator_delete(Sprite_dtor(old));
    F<void *>(self, 0x0) = 0;
    void *spr = operator new(0x40);
    int w = PaintCanvas_GetImage2DWidth(*holder, (int)ids[0]);
    int h = PaintCanvas_GetImage2DHeight(*holder, (int)ids[0]);
    Sprite_ctor(spr, ids, 6, w, h);
    F<void *>(self, 0x0) = spr;
    PaintCanvas_Image2DCreate(*holder, 0x485, (char *)self + 4);
    return ImageFactory_reload_tail(*holder, 0x511, (char *)self + 8);
}
