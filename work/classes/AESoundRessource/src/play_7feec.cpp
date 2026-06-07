#include "class.h"

extern "C" void AESoundRessource_getSoundInfo(AbyssEngine::AESoundRessource *self, int id, AbyssEngine::AESoundInfo *info, int *index);
extern "C" int AESoundRessource_init(AbyssEngine::AESoundRessource *self, int id);
extern "C" int __stack_chk_guard;
extern "C" void __stack_chk_fail(int diff) __attribute__((noreturn));

typedef int (*SoundIntReturnMethod)(void *);
typedef void (*SoundFloatMethod)(void *, float);

extern "C" void AESoundRessource_play_7feec(AbyssEngine::AESoundRessource *self, int id, float volume)
{
    float savedVolume = volume;
    int savedId = id;
    AESoundStackFrame frame;
    frame.cookie = __stack_chk_guard;
    AESoundRessource_getSoundInfo(self, id, (AbyssEngine::AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
        if ((*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x38))(sound) == 0) {
            AESoundRessource_init(self, savedId);
        }
        sound = *(void **)(*(char **)((char *)self + 8) + index * 4);
        if (sound != 0) {
            (*(SoundFloatMethod *)((char *)*(void **)sound + 0xc))(sound, savedVolume);
        }
    }
    int diff = __stack_chk_guard;
    diff -= frame.cookie;
    if (diff != 0) {
        __stack_chk_fail(diff);
    }
}
