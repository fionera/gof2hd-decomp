#include "class.h"

typedef void (*SoundVoidMethod)(void *);

extern "C" void AESoundRessource_resume_800e8(AbyssEngine::AESoundRessource *self)
{
    for (uint32_t i = 0; i < *(uint32_t *)((char *)self + 4); ++i) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + i * 4);
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
}
