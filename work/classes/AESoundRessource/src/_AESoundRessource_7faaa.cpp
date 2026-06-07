#include "class.h"

extern "C" void AESoundRessource_freeAllRessources(AbyssEngine::AESoundRessource *self);
extern "C" void ArrayRemoveAll_AESoundInterface(void *array);
extern "C" void Array_AESoundInterface_dtor(void *array);
extern "C" void operator_delete(void *ptr);

AbyssEngine::AESoundRessource::~AESoundRessource()
{
    AbyssEngine::AESoundRessource *self = this;
    AESoundRessource_freeAllRessources(self);
    void *array = (char *)self + 4;
    int zero = 0;
    int byteOffset = 0;
    uint32_t i = 0;
    while (i < *(uint32_t *)array) {
        char *data = *(char **)((char *)self + 8);
        void *sound = *(void **)(data + byteOffset);
        int *slot;
        if (sound == 0) {
            slot = (int *)(data + i * 4);
        } else {
            operator_delete(sound);
            slot = (int *)(*(char **)((char *)self + 8) + byteOffset);
        }
        *slot = zero;
        byteOffset += 4;
        ++i;
    }
    ArrayRemoveAll_AESoundInterface(array);
    Array_AESoundInterface_dtor(array);
}
