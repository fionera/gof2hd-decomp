#include "class.h"

extern "C" void Array_AESoundInterface_ctor(void *array);

extern "C" AbyssEngine::AESoundRessource *AESoundRessource_7fa78(AbyssEngine::AESoundRessource *self)
{
    Array_AESoundInterface_ctor((char *)self + 4);
    *(volatile int *)((char *)self + 0x10) = 0;
    *(volatile int *)((char *)self + 0) = 0;
    return self;
}
