#include "class.h"

extern "C" void *Sprite_dtor(void *p);
extern "C" void operator_delete(void *p);

// ImageFactory::~ImageFactory() -> returns this. Frees the owned Sprite at +0x00.
extern "C" ImageFactory *_ZN12ImageFactoryD2Ev(ImageFactory *self)
{
    void *p = F<void *>(self, 0x0);
    if (p != 0) operator_delete(Sprite_dtor(p));
    F<void *>(self, 0x0) = 0;
    return self;
}
