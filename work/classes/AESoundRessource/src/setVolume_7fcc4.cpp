#include "class.h"

extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" int __stack_chk_guard;
extern "C" void __stack_chk_fail(int diff) __attribute__((noreturn));

typedef void (*SoundIntMethod)(void *, int);

extern "C" void AESoundRessource_setVolume_7fcc4(AbyssEngine::AESoundRessource *self, int id, int volume)
{
    int savedVolume = volume;
    AESoundStackFrame frame;
    frame.cookie = __stack_chk_guard;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + frame.index * 4);
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, savedVolume);
        }
    }
    int diff = __stack_chk_guard;
    diff -= frame.cookie;
    if (diff != 0) {
        __stack_chk_fail(diff);
    }
}
