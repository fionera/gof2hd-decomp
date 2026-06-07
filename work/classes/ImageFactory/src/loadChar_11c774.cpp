#include "class.h"

struct Arr { uint32_t size; void **data; uint32_t size2; };

extern "C" void Array_ImagePart_ctor(Arr *a);
extern "C" void ArraySetLength_ImagePart(uint32_t n, Arr *a);
extern "C" void *ImageFactory_loadImage(void *thisish, int a, int b, int c);

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
