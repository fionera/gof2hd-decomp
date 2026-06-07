#include "class.h"

extern "C" void AESoundRessource_SetSound(void *sound, void *info, int count);
extern "C" void AESoundRessource_init(void *sound, int index);

__attribute__((minsize)) extern "C" void ApplicationManager_SoundSet(ApplicationManager *self, void *info, int count)
{
    if (info != 0) {
        void *sound = *(void **)((char *)self + 0xac);
        if (sound != 0) {
            AESoundRessource_SetSound(sound, info, count);
            for (int i = 0; i < count; ++i) {
                AESoundRessource_init(*(void **)((char *)self + 0xac), i);
            }
        }
    }
}
