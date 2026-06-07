#include "class.h"

typedef void (*SoundIntMethod)(void *, int);

extern "C" void AESoundRessource_setMusicVolume_7fd70(AbyssEngine::AESoundRessource *self, int volume)
{
    for (uint32_t i = 0; i < *(uint32_t *)((char *)self + 4); ++i) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + i * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x30))(sound, volume);
        }
    }
}
