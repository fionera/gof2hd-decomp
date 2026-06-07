#include "class.h"

extern "C" void operator_delete(void *ptr);

extern "C" void AESoundRessource_freeAllRessources_7fb0c(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < *(uint32_t *)((char *)self + 4); ++i) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + i * 4);
        if (sound != 0) {
            operator_delete(sound);
            *(int *)(*(char **)((char *)self + 8) + i * 4) = 0;
        }
    }
}
